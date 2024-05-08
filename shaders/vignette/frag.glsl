#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float vignetteStrength;

void main() {
    vec2 uv = TexCoords;
    vec2 center = vec2(0.5, 0.5);
    float dist = distance(uv, center);
    float vignette = smoothstep(0.5, 0.8, dist);
    
    vec3 color = texture(screenTexture, TexCoords).rgb;
    color *= (1.0 - vignetteStrength * vignette);
    
    FragColor = vec4(color, 1.0);
}