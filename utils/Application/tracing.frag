#version 130

#define lt1 10
#define lt4 10
#define lt5 12
#define PI 3.14159265359
#define dp 0.0001

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

vec3 get_col(int[2] p)
{
    vec3 col = vec3(0.7,0.7,1);
    if(p[0]==0) col = vec3(sphears[lt1*p[1]+4],sphears[lt1*p[1]+5],sphears[lt1*p[1]+6])*sphears[lt1*p[1]+7]/255;
    if(p[0]==3) col = vec3(planes[lt4*p[1]+4],planes[lt4*p[1]+5],planes[lt4*p[1]+6])*planes[lt4*p[1]+7]/255;
    if(p[0]==4) col = vec3(cubes[lt5*p[1]+6],cubes[lt5*p[1]+7],cubes[lt5*p[1]+8])*cubes[lt5*p[1]+9]/255;
    return col;
}

vec3 rot(inout vec3 a, vec3 al)
{
    al.z = PI*al.z/180;    al.y = PI*al.y/180;
    a = vec3(a.x*cos(al.y)+a.z*sin(al.y), a.y, -a.x*sin(al.y)+a.z*cos(al.y));
    a = vec3(a.x*cos(al.z)-a.y*sin(al.z), a.x*sin(al.z)+a.y*cos(al.z), a.z);
    return a;
}

float sphIntersect(in vec3 ro, in vec3 rd, float ra, out vec3 oN) 
{
    if(dot(ro,ro)<ra*ra) {oN = normalize(ro); return dot(oN,light)>0 ? 0.0 : -1.0;}
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

int[2] raycast(vec3 rp, vec3 rd, inout float dst, out vec3 nv)
{
    int[2] res; res[0]=-1; res[1]=0;
    vec3 oN;
    vec3 sun = vec3(0.3)*dot(rd, light);
    float dh = 0;
    for(int i=0; i<geom[0]; i++)
    {
        dh = sphIntersect(rp-vec3(sphears[lt1*i+0],sphears[lt1*i+1],sphears[lt1*i+2]), rd, sphears[lt1*i+3], oN);
        if(0<=dh && dh<dst) 
        {
            dst=dh; nv=oN; res[0]=0; res[1]=i;
        }
    }
    for(int i=0; i<geom[3]; i++)
    {
        dh = plnIntersect(rp, rd, vec4(planes[lt4*i+0],planes[lt4*i+1],planes[lt4*i+2], planes[lt4*i+3]), oN);
        if(0<=dh && dh<dst) 
        {
            dst=dh; nv=oN; res[0]=3; res[1]=i;
        }
    }
    for(int i=0; i<geom[4]; i++)
    {
        dh = cubIntersect(rp-vec3(cubes[lt5*i+0],cubes[lt5*i+1],cubes[lt5*i+2]), rd, vec3(cubes[lt5*i+3],cubes[lt5*i+4],cubes[lt5*i+5])/2, oN);
        if(0<=dh && dh<dst) 
        {
            dst=dh; nv=oN; res[0]=4; res[1]=i;
        }
    }
    return res;
}

void rastor( float dst, vec3 rd, vec3 nv, int exl[2], int ext[2], inout vec3 col)
{
    if(exl[0] == ext[0] && exl[1] == ext[1]) col *= clamp(pow(1-dot(nv, light), 2), 0.0, 1.0);
    else if(ext[0]>-1) col *= pow((dst/len), 0.2);
    else 
    {
        col = col*1; //(1+0.3*pow(dot(-rd, reflect(light, nv)), 1.0))
        col = clamp(col+vec3(0.15)*max(0.0, pow(dot(-rd, reflect(light, nv)), 50.0)), 0.0, 1.0);
    }
}

vec3 raytrace(vec3 rp, vec3 rd)
{
    vec3 col = vec3(0);
    int[2] exl;
    int[2] ext;
    vec3 nv = vec3(0);
    vec3 sun = vec3(1)*max(0.0, pow(dot(-rd, light), 200.0));
    float dst = len;
    exl = raycast(rp, rd, dst, nv);
    col = get_col(exl);
    if(dst==len) return clamp(col+sun, 0.0, 1.0);
    rp += (dst-dp)*rd;dst = len; //rd = reflect(rd, nv); 

    ext = raycast(rp, -light, dst, sun);
    rastor(dst, rd, nv, exl, ext, col);
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
    // if(planes[7]==1.0) col=vec3(1);
    gl_FragColor = vec4(col, 1.0);
}