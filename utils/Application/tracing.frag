#version 130

#define lt1 7
#define lt4 7
#define lt5 9
#define PI 3.14159265359

uniform vec2 resolution;
uniform vec3 light;
uniform float focus;
uniform float len;
uniform vec3 pos;
uniform vec3 nal;
uniform float geom[5];
uniform float sphears[lt1*2];
uniform float planes[lt4*1];
uniform float cubes[lt5*2];

vec3 rot(inout vec3 a, vec3 al)
{
    al.z = PI*al.z/180;    al.y = PI*al.y/180;
    a = vec3(a.x*cos(al.y)+a.z*sin(al.y), a.y, -a.x*sin(al.y)+a.z*cos(al.y));
    a = vec3(a.x*cos(al.z)-a.y*sin(al.z), a.x*sin(al.z)+a.y*cos(al.z), a.z);
    return a;
}

float sphIntersect(in vec3 ro, in vec3 rd, float ra, out vec3 oN) {
	float b = dot(ro, rd);
	float c = dot(ro, ro) - ra * ra;
	float h = b * b - c;
	if(h < 0.0) return len;
	h = sqrt(h);
    oN = normalize(ro+min(-b - h, -b + h)*rd);
	return min(-b - h, -b + h);
}

float cubIntersect( in vec3 ro, in vec3 rd, vec3 boxSize, out vec3 oN) 
{
    vec3 m = 1.0/rd; 
    vec3 n = m*ro;   
    vec3 k = abs(m)*boxSize;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    float tN = max( max( t1.x, t1.y ), t1.z );
    float tF = min( min( t2.x, t2.y ), t2.z );
    if( tN>tF || tF<0.0) return len;
    oN = (tN>0.0) ? step(vec3(tN),t1) : step(t2,vec3(tF));
    oN *= -sign(rd);
    return min( tN, tF );
}

float plnIntersect( in vec3 ro, in vec3 rd, in vec4 p, out vec3 oN)
{
    float l = -(dot(ro,p.xyz)+p.w)/dot(rd,p.xyz);
    if(l<0) l=len;
    oN = dot(p.xyz,ro)<0 ? -p.xyz : p.xyz;
    return l;
}

vec4 raycast(vec3 rp, vec3 rd, float dst)
{
    vec3 col = vec3(0.7,0.7,1);
    vec3 oN, nv;
    float dh = 0;
    for(int i=0; i<geom[0]; i++)
    {
        dh = sphIntersect(rp-vec3(sphears[lt1*i+0],sphears[lt1*i+1],sphears[lt1*i+2]), rd, sphears[lt1*i+3], oN);
        if(0<dh && dh<dst) {dst=dh; nv=oN; col = vec3(sphears[lt1*i+4],sphears[lt1*i+5],sphears[lt1*i+6])/255;}
    }
    for(int i=0; i<geom[3]; i++)
    {
        dh = plnIntersect(rp, rd, vec4(planes[lt4*i+0],planes[lt4*i+1],planes[lt4*i+2], planes[lt4*i+3]), oN);
        if(0<dh && dh<dst) {dst=dh; nv=oN; col = vec3(planes[lt4*i+4],planes[lt4*i+5],planes[lt4*i+6])/255;}
    }
    for(int i=0; i<geom[4]; i++)
    {
        dh = cubIntersect(rp-vec3(cubes[lt5*i+0],cubes[lt5*i+1],cubes[lt5*i+2]), rd, vec3(cubes[lt5*i+3],cubes[lt5*i+4],cubes[lt5*i+5])/2, oN);
        if(0<dh && dh<dst) {dst=dh; nv=oN; col = vec3(cubes[lt5*i+6],cubes[lt5*i+7],cubes[lt5*i+8])/255;}
    }
    // if(dst<len) col*=0.5-0.5*dot(nv,light);
    // vec3 pnt = rp+rv*dh;1-dst/(len+400.0)
    // if(sphears[0]==600.f) col=vec3(0);
    return vec4(col, dst-0.1);
}

vec3 raytrace(vec3 rp, vec3 rd)
{
    vec3 col = vec3(0);
    vec4 ex = vec4(0);
    vec3 sun = vec3(1)*max(0.0, pow(dot(-rd, light), 200.0));
    ex = raycast(rp, rd, len);
    col = ex.xyz;
    if(ex.w==len-0.1) return clamp(col+sun, 0.0, 1.0);
    rp += ex.w*rd; 
    ex = raycast(rp, -light, len);
    if(ex.w<len-0.1) col *= 0.5*(1+ex.w/len);
    return col;
}


void main()
{
    vec2 uv = (gl_TexCoord[0].xy - 0.5) * resolution / resolution.y;
    vec3 rp = vec3(1.0/focus, uv.x, -uv.y);
    vec3 rd = normalize(rp);
    rp+=pos; rot(rd, nal);
    vec3 col = raytrace(rp, rd);
    col = vec3(pow(col.x, 1/2.2), pow(col.y, 1/2.2), pow(col.z, 1/2.2));
    gl_FragColor = vec4(col, 1.0);
}