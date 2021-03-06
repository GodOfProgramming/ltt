#iChannel0 "file://./thing.png"

vec4 swirly_color();

void main() {
  vec2 begin = gl_FragCoord.xy / iResolution.xy;
  int time = int(iGlobalTime);
  vec2 end = gl_FragCoord.yx;
  vec4 tex = vec4(begin * end * 0.01, 0, 0);
  vec4 color = swirly_color();
  gl_FragColor = color + tex;
}

vec4 swirly_color() {
  float time = iTime * 1.0;
  vec2 uv = (gl_FragCoord.xy / iResolution.xx - 0.5) * 8.0 * iMouse.xy * .03;
  vec2 uv0 = uv;
  float i0 = 1.0;
  float i1 = 1.0;
  float i2 = 1.0;
  float i4 = 0.0;
  for (int s = 0; s < 7; s++) {
    vec2 r;
    r = vec2(cos(uv.y * i0 - i4 + time / i1), sin(uv.x * i0 - i4 + time / i1)) / i2;
    r += vec2(-r.y, r.x) * 0.3;
    uv.xy += r;

    i0 *= 1.93;
    i1 *= 1.15;
    i2 *= 1.7;
    i4 += 0.05 + 0.1 * time * i1;
  }
  float r = sin(uv.x - time) * 0.5 + 0.5;
  float b = sin(uv.y + time) * 0.5 + 0.5;
  float g = sin((uv.x + uv.y + sin(time * 0.5)) * 0.5) * 0.5 + 0.5;

  return vec4(r, g, b, 1) * 0.55;
}
