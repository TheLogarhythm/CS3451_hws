#version 330 core

uniform vec2 iResolution;               /* screen resolution, value passed from CPU */
uniform float iTime;                    /* current time, value passed from CPU */
uniform int iFrame;                     /* current frame, value passed from CPU */
in vec2 fragCoord;                      /* fragment shader input: fragment coordinates, valued passed from vertex shader */
out vec4 fragColor;                     /* fragment shader output: fragment color, value passed to pixel processing for screen display */

const float M_PI = 3.1415926535;                        /* const value for PI */
const vec3 BG_COLOR = vec3(184, 243, 255) / 255.;       /* const value for background color */

//// This function converts from Polar Coordinates to Cartesian coordinates

vec2 polar2cart(float angle, float length) {
    return vec2(cos(angle) * length, sin(angle) * length);
}

//// This is a sample function showing you how to check if a point is inside a triangle

bool inTriangle(vec2 p, vec2 p1, vec2 p2, vec2 p3) {
    if(dot(cross(vec3(p2 - p1, 0), vec3(p - p1, 0)), cross(vec3(p2 - p1, 0), vec3(p3 - p1, 0))) >= 0. &&
        dot(cross(vec3(p3 - p2, 0), vec3(p - p2, 0)), cross(vec3(p3 - p2, 0), vec3(p1 - p2, 0))) >= 0. &&
        dot(cross(vec3(p1 - p3, 0), vec3(p - p3, 0)), cross(vec3(p1 - p3, 0), vec3(p2 - p3, 0))) >= 0.) {
        return true;
    }
    return false;
}

//// This is a sample function showing you how to draw a rotated triangle 
//// Time is specified with iTime

vec3 drawTriangle(vec2 pos, vec2 center, vec3 color) {
    vec2 p1 = polar2cart(iTime * 2, 160.) + center;
    vec2 p2 = polar2cart(iTime * 2 + 2. * M_PI / 3., 160.) + center;
    vec2 p3 = polar2cart(iTime * 2 + 4. * M_PI / 3., 160.) + center;
    if(inTriangle(pos, p1, p2, p3)) {
        return color;
    }
    return vec3(0);
}

/////////////////////////////////////////////////////
//// Step 1 Function: Inside Circle
//// In this function, you will implement a function to checks if a point is inside a circle
//// The inputs include the point position, the circle's center and radius
//// The output is a bool indicating if the point is inside the circle (true) or not (false)
/////////////////////////////////////////////////////
//// Implementation hint: use dot(v,v) to calculate the squared length of a vector v
/////////////////////////////////////////////////////

bool inCircle(vec2 pos, vec2 center, float radius) {
    /* your implementation starts */
    if(dot(pos - center, pos - center) <= radius * radius) {
        return true;
    }
    /* your implementation ends */

    return false;
}

//// This function calls the inCircle function you implemented above and returns the color of the circle
//// If the point is outside the circle, it returns a zero vector by default
vec3 drawCircle(vec2 pos, vec2 center, float radius, vec3 color) {
    if(inCircle(pos, center, radius)) {
        return color;
    }
    return vec3(0);
}

/////////////////////////////////////////////////////
//// Step 2 Function: Inside Rectangle
//// In this function, you will implement a function to checks if a point is inside a rectangle
//// The inputs include the point position, the left bottom corner and the right top corner of the rectangle
//// The output is a bool indicating if the point is inside the rectangle (true) or not (false)
/////////////////////////////////////////////////////
//// Implementation hint: use .x and .y to access the x and y components of a vec2 variable
/////////////////////////////////////////////////////

bool inRectangle(vec2 pos, vec2 leftBottom, vec2 rightTop) {
    /* your implementation starts */
    if(pos.x >= leftBottom.x && pos.x <= rightTop.x && pos.y >= leftBottom.y && pos.y <= rightTop.y) {
        return true;
    }
    /* your implementation ends */

    return false;
}

//// This function calls the inRectangle function you implemented above and returns the color of the rectangle
//// If the point is outside the rectangle, it returns a zero vector by default

vec3 drawRectangle(vec2 pos, vec2 leftBottom, vec2 rightTop, vec3 color) {
    if(inRectangle(pos, leftBottom, rightTop)) {
        return color;
    }
    return vec3(0);
}

//// Ring shape
bool inRing(vec2 pos, vec2 center, float innerRadius, float outerRadius) {
    if(dot(pos - center, pos - center) <= outerRadius * outerRadius &&
        dot(pos - center, pos - center) >= innerRadius * innerRadius) {
        return true;
    }
    return false;
}

vec3 drawRing(vec2 pos, vec2 center, float innerRadius, float outerRadius, vec3 color) {
    if(inRing(pos, center, innerRadius, outerRadius)) {
        return color;
    }
    return vec3(0);
}

//// Star shape, made by 2 overlapping triangles
bool inStar(vec2 pos, vec2 center, float size, float angle) {
    vec2 p1 = polar2cart(angle, size) + center;
    vec2 p2 = polar2cart(angle + 2. * M_PI / 3., size) + center;
    vec2 p3 = polar2cart(angle + 4. * M_PI / 3., size) + center;

    vec2 p4 = polar2cart(angle + M_PI / 3., size) + center;
    vec2 p5 = polar2cart(angle + M_PI, size) + center;
    vec2 p6 = polar2cart(angle + 5. * M_PI / 3., size) + center;
    if(inTriangle(pos, p1, p2, p3) || inTriangle(pos, p4, p5, p6)) {
        return true;
    }
    return false;
}

vec3 drawStar(vec2 pos, vec2 center, float size, float rotationSpeed, vec3 color) {
    float angle = iTime * rotationSpeed;
    if(inStar(pos, center, size, angle)) {
        return color;
    }
    return vec3(0);
}

//// This function draws objects on the canvas by specifying a fragColor for each fragCoord

void mainImage(in vec2 fragCoord, out vec4 fragColor) {
    //// Get the window center
    vec2 center = vec2(iResolution / 2.);

    //// By default we draw an animated triangle 
    vec3 fragOutput = drawTriangle(fragCoord, center, vec3(1.0));

    // // Step 1: Uncomment this line to draw a circle
    // fragOutput = drawCircle(fragCoord, center, 250, vec3(1.0));

    // // Step 2: Uncomment this line to draw a rectangle 
    // fragOutput = drawRectangle(fragCoord, center - vec2(500, 50), center + vec2(500, 50), vec3(1.0));

    // // Step 3: Uncomment this line to draw an animated circle with a temporally varying radius controlled by a sine function
    // fragOutput = drawCircle(fragCoord, center, 150 + 50. * sin(iTime * 10), vec3(1.0));

    // // Step 4: Uncomment this line to draw a union of the rectangle and the animated circle you have drawn previously
    // fragOutput = drawRectangle(fragCoord, center - vec2(500, 50), center + vec2(500, 50), vec3(1.0)) + drawCircle(fragCoord, center, 150 + 50. * sin(iTime * 10), vec3(1.0));

    //// Set the fragment color to be the background color if it is zero
    if(fragOutput == vec3(0)) {
        fragColor = vec4(BG_COLOR, 1.0);
    }
    //// Otherwise set the fragment color to be the color calculated in fragOutput
    else {
        fragColor = vec4(fragOutput, 1.0);
    }

    //// Step 5: Implement your customized scene by modifying the mainImage function
    //// Try to leverage what you have learned from Step 1 to 4 to define the shape and color of a new object in the fragment shader
    //// Notice how we put multiple objects together by adding their color values

    //// Idea: Solar system + starry sky

    // 1. The sun 太阳 - 脉动效果
    vec3 sunColor = vec3(1.0, 0.18, 0.0);
    float sunRadius = 1.0 + 0.1 * sin(iTime * 3);
    fragOutput = drawCircle(fragCoord, center, 25 * sunRadius, sunColor);

    // Halo around the sun
    fragOutput += drawRing(fragCoord, center, 25 * sunRadius, 30 * sunRadius, sunColor * 0.3);

    // 2. Orbit paths (gray rings)
    vec3 orbitColor = vec3(0.45);
    fragOutput += drawRing(fragCoord, center, 70.0 - 0.5, 70.0 + 0.5, orbitColor);   // 水星轨道
    fragOutput += drawRing(fragCoord, center, 110.0 - 0.5, 110.0 + 0.5, orbitColor);   // 金星轨道
    fragOutput += drawRing(fragCoord, center, 150.0 - 0.5, 150.0 + 0.5, orbitColor);   // 地球轨道
    fragOutput += drawRing(fragCoord, center, 210.0 - 0.5, 210.0 + 0.5, orbitColor); // 火星轨道
    fragOutput += drawRing(fragCoord, center, 270.0 - 0.5, 270.0 + 0.5, orbitColor); // 木星轨道
    fragOutput += drawRing(fragCoord, center, 330.0 - 0.5, 330.0 + 0.5, orbitColor); // 土星轨道
    fragOutput += drawRing(fragCoord, center, 395.0 - 0.5, 395.0 + 0.5, orbitColor); // 天王星轨道
    fragOutput += drawRing(fragCoord, center, 440.0 - 0.5, 440.0 + 0.5, orbitColor); // 海王星轨道

    // 3. Planets

    // Mercury 水星
    vec3 mercuryColor = vec3(0.46, 0.36, 0.31);
    vec2 mercury = polar2cart(iTime * 3.0, 70.0) + center;
    fragOutput += drawCircle(fragCoord, mercury, 5.0, mercuryColor);

    // Venus 金星
    vec3 venusColor = vec3(0.98, 0.78, 0.55);
    vec2 venus = polar2cart(iTime * 2.2, 110.0) + center;
    fragOutput += drawCircle(fragCoord, venus, 6.0, venusColor);

    // Earth 地球
    vec3 earthColor = vec3(0.0, 0.5, 1.0);
    vec2 earth = polar2cart(iTime * 1.5, 150.0) + center;
    fragOutput += drawCircle(fragCoord, earth, 8.0, earthColor);

    // Moon 月球
    vec3 moonColor = vec3(0.8, 0.8, 0.8);
    vec2 moon = polar2cart(iTime * 20 + 0.5, 20.0) + earth;
    fragOutput += drawCircle(fragCoord, moon, 2.0, moonColor);

    // Mars 火星
    vec3 marsColor = vec3(0.8, 0.2, 0.2);
    vec2 mars = polar2cart(iTime * 1.2, 210.0) + center;
    fragOutput += drawCircle(fragCoord, mars, 6.0, marsColor);

    // Jupiter 木星
    vec3 jupiterColor = vec3(0.73, 0.45, 0.16);
    vec2 jupiter = polar2cart(iTime * 1.0, 270.0) + center;
    fragOutput += drawCircle(fragCoord, jupiter, 13.0, jupiterColor);

    // Saturn 土星
    vec3 saturnColor = vec3(0.98, 0.81, 0.67);
    vec2 saturn = polar2cart(iTime * 0.8, 330.0) + center;
    fragOutput += drawCircle(fragCoord, saturn, 15.0, saturnColor);
    // Saturn's rings
    fragOutput += drawRing(fragCoord, saturn, 22.0, 23.5, vec3(0.84, 0.81, 0.78));
    fragOutput += drawRing(fragCoord, saturn, 26.0, 27.5, vec3(0.42, 0.41, 0.39));

    // Uranus 天王星
    vec3 uranusColor = vec3(0.56, 0.81, 0.89);
    vec2 uranus = polar2cart(iTime * 0.6, 395.0) + center;
    fragOutput += drawCircle(fragCoord, uranus, 8.0, uranusColor);

    // Neptune 海王星
    vec3 neptuneColor = vec3(0.18, 0.38, 0.96);
    vec2 neptune = polar2cart(iTime * 0.5, 440.0) + center;
    fragOutput += drawCircle(fragCoord, neptune, 8.0, neptuneColor);

    // 4. Background stars
    vec3 starColor1 = vec3(0.97, 0.94, 0.0);
    vec3 starColor2 = vec3(0.99, 0.98, 0.75);
    fragOutput += drawStar(fragCoord, center + vec2(-500, 300), 15.0, 2.0, starColor1);
    fragOutput += drawStar(fragCoord, center + vec2(-400, 350), 10.0, 1.0, starColor2);
    fragOutput += drawStar(fragCoord, center + vec2(420, -300), 13.0, 1.5, starColor1);

    const vec3 self_BG_COLOR = vec3(0.0, 0.0, 0.09);
    if(fragOutput == vec3(0)) {
        fragColor = vec4(self_BG_COLOR, 1.0);
    } else {
        fragColor = vec4(fragOutput, 1.0);
    }
}

void main() {
    mainImage(fragCoord, fragColor);
}