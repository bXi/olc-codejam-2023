#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

uniform float iTime;

const int maxLights = 160; //255 = max

struct Light {
    vec2 firstPos;
    vec2 secondPos;
    vec2 thirdPos;
    vec2 fourthPos;
};

uniform Light spots[maxLights];      // Spotlight positions array

// Input uniform values
uniform sampler2D texture0;
uniform vec2 iMouse;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables
const vec2 iResolution = vec2(1280.0, 720.0);
const float missileRadius = 4.0 / iResolution.x;
const float ratio = iResolution.x / iResolution.y;

const float halfR = missileRadius / 2.0;

float lightDisc(in vec2 p, in float r) {
    float dd = dot(p, p);
    if (dd * 2.0 > r) return 0.0;
    if (dd < r * r) return 1.0;
    return (1.0 - acos(halfR / sqrt(dd)) / 1.5257);
}


// SDFs

float sdDisc(in vec2 p, in float r) {
    return length(p) - r;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 uv = fragCoord.xy / iResolution.xy;

    uv.x *= ratio;

    vec3 color = texture(texture0, fragTexCoord.xy).rgb;

    fragColor = vec4(color, 1.0);

    for (int i; i < maxLights; i++) {
        if (spots[i].firstPos.x < -0.1f) continue;
        if (sdDisc(uv - vec2(spots[i].firstPos.x, spots[i].firstPos.y), missileRadius) < 0.0) fragColor.a = 0.0;
        else if (sdDisc(uv - vec2(spots[i].secondPos.x, spots[i].secondPos.y), missileRadius) < 0.0) fragColor.a = 0.0;
        else if (sdDisc(uv - vec2(spots[i].thirdPos.x, spots[i].thirdPos.y), missileRadius) < 0.0) fragColor.a = 0.0;
        else if (sdDisc(uv - vec2(spots[i].fourthPos.x, spots[i].fourthPos.y), missileRadius) < 0.0) fragColor.a = 0.0;
        
        else {
            fragColor.rgb += lightDisc(uv - vec2(spots[i].firstPos.x, spots[i].firstPos.y), missileRadius) * vec3(5.2, 0.0, 3.0);
            fragColor.rgb += lightDisc(uv - vec2(spots[i].secondPos.x, spots[i].secondPos.y), missileRadius) * vec3(5.2, 0.0, 3.0);
            fragColor.rgb += lightDisc(uv - vec2(spots[i].thirdPos.x, spots[i].thirdPos.y), missileRadius) * vec3(5.2, 0.0, 3.0);
            fragColor.rgb += lightDisc(uv - vec2(spots[i].fourthPos.x, spots[i].fourthPos.y), missileRadius) * vec3(5.2, 0.0, 3.0);
        }
    }
    //fragColor = finalColor;


}

void main(void)
{

    finalColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    mainImage(color, gl_FragCoord.xy);
    if (finalColor.x < 0.0) color = vec4(1.0, 0.0, 0.0, 1.0);
    if (finalColor.y < 0.0) color = vec4(0.0, 1.0, 0.0, 1.0);
    if (finalColor.z < 0.0) color = vec4(0.0, 0.0, 1.0, 1.0);
    if (finalColor.w < 0.0) color = vec4(1.0, 1.0, 0.0, 1.0);
    finalColor = vec4(color.xyz, 1.0);
}