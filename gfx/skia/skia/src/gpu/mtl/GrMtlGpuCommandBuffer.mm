/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrMtlGpuCommandBuffer.h"

#include "GrColor.h"
#include "GrFixedClip.h"
#include "GrMtlPipelineState.h"
#include "GrMtlPipelineStateBuilder.h"
#include "GrMtlRenderTarget.h"
#include "GrRenderTargetPriv.h"

GrMtlGpuRTCommandBuffer::GrMtlGpuRTCommandBuffer(
        GrMtlGpu* gpu, GrRenderTarget* rt, GrSurfaceOrigin origin, const SkRect& bounds,
        const GrGpuRTCommandBuffer::LoadAndStoreInfo& colorInfo,
        const GrGpuRTCommandBuffer::StencilLoadAndStoreInfo& stencilInfo)
        : INHERITED(rt, origin)
        , fGpu(gpu)
#ifdef SK_DEBUG
        , fBounds(bounds)
#endif
        , fColorLoadAndStoreInfo(colorInfo)
        , fStencilLoadAndStoreInfo(stencilInfo)
        , fRenderPassDesc(this->createRenderPassDesc()) {
    (void)fStencilLoadAndStoreInfo; // Silence unused var warning
    const GrMtlStencilAttachment* stencil = static_cast<GrMtlStencilAttachment*>(
                                                     rt->renderTargetPriv().getStencilAttachment());
    if (stencil) {
        fRenderPassDesc.stencilAttachment.texture = stencil->stencilView();
    }
    if (fColorLoadAndStoreInfo.fLoadOp == GrLoadOp::kClear) {
        fCommandBufferInfo.fBounds = SkRect::MakeWH(fRenderTarget->width(),
                                                    fRenderTarget->height());
        this->addNullCommand();
        fRenderPassDesc.colorAttachments[0].loadAction = MTLLoadActionLoad;
    } else {
        fCommandBufferInfo.fBounds.setEmpty();
    }
    switch (stencilInfo.fLoadOp) {
        case GrLoadOp::kLoad:
            fRenderPassDesc.stencilAttachment.loadAction = MTLLoadActionLoad;
            break;
        case GrLoadOp::kClear:
            fCommandBufferInfo.fBounds = SkRect::MakeWH(fRenderTarget->width(),
                                                        fRenderTarget->height());
            fRenderPassDesc.stencilAttachment.loadAction = MTLLoadActionClear;
            this->addNullCommand();
            fRenderPassDesc.stencilAttachment.loadAction = MTLLoadActionLoad;
            break;
        case GrLoadOp::kDiscard:
            fRenderPassDesc.stencilAttachment.loadAction = MTLLoadActionDontCare;
            break;
    }
    switch (stencilInfo.fStoreOp) {
        case GrStoreOp::kStore:
            fRenderPassDesc.stencilAttachment.storeAction = MTLStoreActionStore;
            break;
        case GrStoreOp::kDiscard:
            fRenderPassDesc.stencilAttachment.storeAction = MTLStoreActionDontCare;
            break;
    }
}

GrMtlGpuRTCommandBuffer::~GrMtlGpuRTCommandBuffer() {
    SkASSERT(fActiveRenderCmdEncoder == nil);
}

void GrMtlGpuRTCommandBuffer::addNullCommand() {
    SkASSERT(fActiveRenderCmdEncoder == nil);
    fActiveRenderCmdEncoder = [fGpu->commandBuffer()
                               renderCommandEncoderWithDescriptor:fRenderPassDesc];
    SkASSERT(fActiveRenderCmdEncoder);
    [fActiveRenderCmdEncoder endEncoding];
    fActiveRenderCmdEncoder = nil;
    SkASSERT(fActiveRenderCmdEncoder == nil);
}

void GrMtlGpuRTCommandBuffer::submit() {
    if (!fRenderTarget) {
        return;
    }
    SkIRect iBounds;
    fCommandBufferInfo.fBounds.roundOut(&iBounds);
    fGpu->submitIndirectCommandBuffer(fRenderTarget, fOrigin, &iBounds);
}

void GrMtlGpuRTCommandBuffer::copy(GrSurface* src, GrSurfaceOrigin srcOrigin,
                                   const SkIRect& srcRect, const SkIPoint& dstPoint) {
    // We cannot have an active encoder when we call copy since it requires its own
    // command encoder.
    SkASSERT(fActiveRenderCmdEncoder == nil);
    fGpu->copySurface(fRenderTarget, fOrigin, src, srcOrigin, srcRect, dstPoint);
}

GrMtlPipelineState* GrMtlGpuRTCommandBuffer::prepareDrawState(
        const GrPrimitiveProcessor& primProc,
        const GrPipeline& pipeline,
        const GrPipeline::FixedDynamicState* fixedDynamicState,
        GrPrimitiveType primType) {
    // TODO: resolve textures and regenerate mipmaps as needed

    const GrTextureProxy* const* primProcProxies = nullptr;
    if (fixedDynamicState) {
        primProcProxies = fixedDynamicState->fPrimitiveProcessorTextures;
    }
    SkASSERT(SkToBool(primProcProxies) == SkToBool(primProc.numTextureSamplers()));

    GrMtlPipelineState* pipelineState =
        fGpu->resourceProvider().findOrCreateCompatiblePipelineState(fRenderTarget, fOrigin,
                                                                     pipeline,
                                                                     primProc,
                                                                     primProcProxies,
                                                                     primType);
    if (!pipelineState) {
        return nullptr;
    }
    // We cannot have an active encoder when we set the pipeline data since it requires its own
    // command encoder.
    // TODO: this doesn't appear to be true -- we only use a command encoder here
    // SkASSERT(fActiveRenderCmdEncoder == nil);
    pipelineState->setData(fRenderTarget, fOrigin, primProc, pipeline, primProcProxies);

    return pipelineState;
}

void GrMtlGpuRTCommandBuffer::onDraw(const GrPrimitiveProcessor& primProc,
                                     const GrPipeline& pipeline,
                                     const GrPipeline::FixedDynamicState* fixedDynamicState,
                                     const GrPipeline::DynamicStateArrays* dynamicStateArrays,
                                     const GrMesh meshes[],
                                     int meshCount,
                                     const SkRect& bounds) {
    if (!meshCount) {
        return;
    }
    if (pipeline.isScissorEnabled()) {
        return; // TODO: ScissorRects are not supported.
    }

    GrPrimitiveType primitiveType = meshes[0].primitiveType();
    GrMtlPipelineState* pipelineState = this->prepareDrawState(primProc, pipeline,
                                                               fixedDynamicState, primitiveType);
    if (!pipelineState) {
        return;
    }

    SkASSERT(fActiveRenderCmdEncoder == nil);
    fActiveRenderCmdEncoder = [fGpu->commandBuffer()
                               renderCommandEncoderWithDescriptor:fRenderPassDesc];
    SkASSERT(fActiveRenderCmdEncoder);
    // TODO: can we set this once somewhere at the beginning of the draw?
    [fActiveRenderCmdEncoder setFrontFacingWinding:MTLWindingCounterClockwise];

    [fActiveRenderCmdEncoder setRenderPipelineState:pipelineState->mtlPipelineState()];
    pipelineState->bind(fActiveRenderCmdEncoder);
    pipelineState->setBlendConstants(fActiveRenderCmdEncoder, fRenderTarget->config(),
                                     pipeline.getXferProcessor());
    pipelineState->setDepthStencilState(fActiveRenderCmdEncoder);

    for (int i = 0; i < meshCount; ++i) {
        const GrMesh& mesh = meshes[i];
        SkASSERT(nil != fActiveRenderCmdEncoder);
        if (mesh.primitiveType() != primitiveType) {
            SkDEBUGCODE(pipelineState = nullptr);
            primitiveType = mesh.primitiveType();
            pipelineState = this->prepareDrawState(primProc, pipeline, fixedDynamicState,
                                                   primitiveType);
            if (!pipelineState) {
                return;
            }

            [fActiveRenderCmdEncoder setRenderPipelineState: pipelineState->mtlPipelineState()];
            pipelineState->bind(fActiveRenderCmdEncoder);
            pipelineState->setBlendConstants(fActiveRenderCmdEncoder, fRenderTarget->config(),
                                             pipeline.getXferProcessor());
            pipelineState->setDepthStencilState(fActiveRenderCmdEncoder);
        }

        mesh.sendToGpu(this);
    }

    [fActiveRenderCmdEncoder endEncoding];
    fActiveRenderCmdEncoder = nil;
    fCommandBufferInfo.fBounds.join(bounds);
}

void GrMtlGpuRTCommandBuffer::onClear(const GrFixedClip& clip, const SkPMColor4f& color) {
    // if we end up here from absClear, the clear bounds may be bigger than the RT proxy bounds -
    // but in that case, scissor should be enabled, so this check should still succeed
    SkASSERT(!clip.scissorEnabled() || clip.scissorRect().contains(fBounds));
    fRenderPassDesc.colorAttachments[0].clearColor = MTLClearColorMake(color.fR, color.fG, color.fB,
                                                                       color.fA);
    fRenderPassDesc.colorAttachments[0].loadAction = MTLLoadActionClear;
    this->addNullCommand();
    fRenderPassDesc.colorAttachments[0].loadAction = MTLLoadActionLoad;
}

void GrMtlGpuRTCommandBuffer::onClearStencilClip(const GrFixedClip& clip, bool insideStencilMask) {
    SkASSERT(!clip.hasWindowRectangles());

    GrStencilAttachment* sb = fRenderTarget->renderTargetPriv().getStencilAttachment();
    // this should only be called internally when we know we have a
    // stencil buffer.
    SkASSERT(sb);
    int stencilBitCount = sb->bits();

    // The contract with the callers does not guarantee that we preserve all bits in the stencil
    // during this clear. Thus we will clear the entire stencil to the desired value.
    if (insideStencilMask) {
        fRenderPassDesc.stencilAttachment.clearStencil = (1 << (stencilBitCount - 1));
    } else {
        fRenderPassDesc.stencilAttachment.clearStencil = 0;
    }

    fRenderPassDesc.stencilAttachment.loadAction = MTLLoadActionClear;
    this->addNullCommand();
    fRenderPassDesc.stencilAttachment.loadAction = MTLLoadActionLoad;
}

MTLRenderPassDescriptor* GrMtlGpuRTCommandBuffer::createRenderPassDesc() const {
    const static MTLLoadAction mtlLoadAction[] {
        MTLLoadActionLoad,
        MTLLoadActionClear,
        MTLLoadActionDontCare
    };
    GR_STATIC_ASSERT((int)GrLoadOp::kLoad == 0);
    GR_STATIC_ASSERT((int)GrLoadOp::kClear == 1);
    GR_STATIC_ASSERT((int)GrLoadOp::kDiscard == 2);
    SkASSERT(fColorLoadAndStoreInfo.fLoadOp <= GrLoadOp::kDiscard);

    const static MTLStoreAction mtlStoreAction[] {
        MTLStoreActionStore,
        MTLStoreActionDontCare
    };
    GR_STATIC_ASSERT((int)GrStoreOp::kStore == 0);
    GR_STATIC_ASSERT((int)GrStoreOp::kDiscard == 1);
    SkASSERT(fColorLoadAndStoreInfo.fStoreOp <= GrStoreOp::kDiscard);

    auto renderPassDesc = [[MTLRenderPassDescriptor alloc] init];
    renderPassDesc.colorAttachments[0].texture =
            static_cast<GrMtlRenderTarget*>(fRenderTarget)->mtlRenderTexture();
    renderPassDesc.colorAttachments[0].slice = 0;
    renderPassDesc.colorAttachments[0].level = 0;
    const SkPMColor4f& clearColor = fColorLoadAndStoreInfo.fClearColor;
    renderPassDesc.colorAttachments[0].clearColor =
            MTLClearColorMake(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    renderPassDesc.colorAttachments[0].loadAction =
            mtlLoadAction[static_cast<int>(fColorLoadAndStoreInfo.fLoadOp)];
    renderPassDesc.colorAttachments[0].storeAction =
            mtlStoreAction[static_cast<int>(fColorLoadAndStoreInfo.fStoreOp)];
    return renderPassDesc;
}

static MTLPrimitiveType gr_to_mtl_primitive(GrPrimitiveType primitiveType) {
    const static MTLPrimitiveType mtlPrimitiveType[] {
        MTLPrimitiveTypeTriangle,
        MTLPrimitiveTypeTriangleStrip,
        MTLPrimitiveTypePoint,
        MTLPrimitiveTypeLine,
        MTLPrimitiveTypeLineStrip
    };
    GR_STATIC_ASSERT((int)GrPrimitiveType::kTriangles == 0);
    GR_STATIC_ASSERT((int)GrPrimitiveType::kTriangleStrip == 1);
    GR_STATIC_ASSERT((int)GrPrimitiveType::kPoints == 2);
    GR_STATIC_ASSERT((int)GrPrimitiveType::kLines == 3);
    GR_STATIC_ASSERT((int)GrPrimitiveType::kLineStrip == 4);

    SkASSERT(primitiveType <= GrPrimitiveType::kLineStrip);
    return mtlPrimitiveType[static_cast<int>(primitiveType)];
}

void GrMtlGpuRTCommandBuffer::bindGeometry(const GrBuffer* vertexBuffer,
                                           const GrBuffer* instanceBuffer) {
    size_t bufferIndex = GrMtlUniformHandler::kLastUniformBinding + 1;
    if (vertexBuffer) {
        SkASSERT(!vertexBuffer->isCpuBuffer());
        SkASSERT(!static_cast<const GrGpuBuffer*>(vertexBuffer)->isMapped());

        auto mtlVertexBuffer = static_cast<const GrMtlBuffer*>(vertexBuffer)->mtlBuffer();
        SkASSERT(mtlVertexBuffer);
        [fActiveRenderCmdEncoder setVertexBuffer: mtlVertexBuffer
                                          offset: 0
                                         atIndex: bufferIndex++];
    }
    if (instanceBuffer) {
        SkASSERT(!instanceBuffer->isCpuBuffer());
        SkASSERT(!static_cast<const GrGpuBuffer*>(instanceBuffer)->isMapped());

        auto mtlInstanceBuffer = static_cast<const GrMtlBuffer*>(instanceBuffer)->mtlBuffer();
        SkASSERT(mtlInstanceBuffer);
        [fActiveRenderCmdEncoder setVertexBuffer: mtlInstanceBuffer
                                          offset: 0
                                         atIndex: bufferIndex++];
    }
}

void GrMtlGpuRTCommandBuffer::sendInstancedMeshToGpu(GrPrimitiveType primitiveType,
                                                     const GrBuffer* vertexBuffer,
                                                     int vertexCount,
                                                     int baseVertex,
                                                     const GrBuffer* instanceBuffer,
                                                     int instanceCount,
                                                     int baseInstance) {
    this->bindGeometry(vertexBuffer, instanceBuffer);

    SkASSERT(primitiveType != GrPrimitiveType::kLinesAdjacency); // Geometry shaders not supported.
    [fActiveRenderCmdEncoder drawPrimitives: gr_to_mtl_primitive(primitiveType)
                                vertexStart: baseVertex
                                vertexCount: vertexCount
                              instanceCount: instanceCount
                               baseInstance: baseInstance];
}

void GrMtlGpuRTCommandBuffer::sendIndexedInstancedMeshToGpu(GrPrimitiveType primitiveType,
                                                            const GrBuffer* indexBuffer,
                                                            int indexCount,
                                                            int baseIndex,
                                                            const GrBuffer* vertexBuffer,
                                                            int baseVertex,
                                                            const GrBuffer* instanceBuffer,
                                                            int instanceCount,
                                                            int baseInstance,
                                                            GrPrimitiveRestart restart) {
    this->bindGeometry(vertexBuffer, instanceBuffer);

    SkASSERT(primitiveType != GrPrimitiveType::kLinesAdjacency); // Geometry shaders not supported.
    id<MTLBuffer> mtlIndexBuffer;
    if (indexBuffer) {
        SkASSERT(!indexBuffer->isCpuBuffer());
        SkASSERT(!static_cast<const GrGpuBuffer*>(indexBuffer)->isMapped());

        mtlIndexBuffer = static_cast<const GrMtlBuffer*>(indexBuffer)->mtlBuffer();
        SkASSERT(mtlIndexBuffer);
    }

    SkASSERT(restart == GrPrimitiveRestart::kNo);
    [fActiveRenderCmdEncoder drawIndexedPrimitives: gr_to_mtl_primitive(primitiveType)
                                        indexCount: indexCount
                                         indexType: MTLIndexTypeUInt16
                                       indexBuffer: mtlIndexBuffer
                                 indexBufferOffset: sizeof(uint16_t) * baseIndex
                                     instanceCount: instanceCount
                                        baseVertex: baseVertex
                                      baseInstance: baseInstance];
}
