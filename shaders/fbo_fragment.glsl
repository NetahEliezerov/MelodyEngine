#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float vignetteIntensity;
uniform float vignetteRadius;
uniform float vignetteSmooth;
uniform vec2 screenSize;
uniform float exposure;
uniform float time;
uniform float gamma;
uniform float grainIntensity;
uniform float chromaticAberrationOffset;

float random(vec2 uv) {
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec2 uv = TexCoords;
    vec2 center = vec2(0.5, 0.5);
    
    // Apply chromatic aberration
    float chromaticAberration = length(uv - center) * chromaticAberrationOffset;
    vec4 colorR = texture(screenTexture, uv + vec2(chromaticAberration, 0.0));
    vec4 colorG = texture(screenTexture, uv);
    vec4 colorB = texture(screenTexture, uv - vec2(chromaticAberration, 0.0));
    vec4 color = vec4(colorR.r, colorG.g, colorB.b, 1.0);
    
    // Calculate the distance from the fragment to each edge of the screen
    float distLeft = uv.x;
    float distRight = 1.0 - uv.x;
    float distTop = uv.y;
    float distBottom = 1.0 - uv.y;
    
    // Calculate the minimum distance to any edge
    float minDist = min(min(distLeft, distRight), min(distTop, distBottom));
    
    // Calculate the vignette effect based on the minimum distance
    float vignette = smoothstep(vignetteRadius, vignetteRadius + vignetteSmooth, minDist);
    
    // Apply bloom effect
    vec3 hdrColor = color.rgb;
  
    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
  
    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));
    
    // Apply vignette and bloom
    color.rgb = mix(mapped, mapped * (1.0 - vignetteIntensity), vignette);
    
    // Add film grain
    float grain = random(uv + vec2(time)) * grainIntensity;
    color.rgb += grain;
    
    FragColor = vec4(color.rgb, 1.0);
}