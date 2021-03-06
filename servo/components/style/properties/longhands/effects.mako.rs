/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

<%namespace name="helpers" file="/helpers.mako.rs" />

// Box-shadow, etc.
<% data.new_style_struct("Effects", inherited=False) %>

${helpers.predefined_type(
    "opacity",
    "Opacity",
    "1.0",
    animation_value_type="ComputedValue",
    flags="CREATES_STACKING_CONTEXT APPLIES_TO_PLACEHOLDER \
           CAN_ANIMATE_ON_COMPOSITOR",
    spec="https://drafts.csswg.org/css-color/#opacity",
    servo_restyle_damage = "reflow_out_of_flow",
)}

${helpers.predefined_type(
    "box-shadow",
    "BoxShadow",
    None,
    vector=True,
    simple_vector_bindings=True,
    animation_value_type="AnimatedBoxShadowList",
    vector_animation_type="with_zero",
    extra_prefixes="webkit",
    ignored_when_colors_disabled=True,
    flags="APPLIES_TO_FIRST_LETTER",
    spec="https://drafts.csswg.org/css-backgrounds/#box-shadow",
)}

${helpers.predefined_type(
    "clip",
    "ClipRectOrAuto",
    "computed::ClipRectOrAuto::auto()",
    animation_value_type="ComputedValue",
    boxed=True,
    allow_quirks=True,
    spec="https://drafts.fxtf.org/css-masking/#clip-property",
)}

${helpers.predefined_type(
    "filter",
    "Filter",
    None,
    vector=True,
    separator="Space",
    animation_value_type="AnimatedFilterList",
    vector_animation_type="with_zero",
    extra_prefixes="webkit",
    flags="CREATES_STACKING_CONTEXT FIXPOS_CB",
    spec="https://drafts.fxtf.org/filters/#propdef-filter",
)}

${helpers.single_keyword(
    "mix-blend-mode",
    """normal multiply screen overlay darken lighten color-dodge
    color-burn hard-light soft-light difference exclusion hue
    saturation color luminosity""",
    gecko_constant_prefix="NS_STYLE_BLEND",
    animation_value_type="discrete",
    flags="CREATES_STACKING_CONTEXT",
    spec="https://drafts.fxtf.org/compositing/#propdef-mix-blend-mode",
)}
