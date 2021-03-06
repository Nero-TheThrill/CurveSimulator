#version 450 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPosition;
in vec2 TexCoord;
in vec3 pos;
in vec3 norm;

uniform bool item_selected;


uniform bool should_use_gpuside_uv;
uniform bool mapping_with_normal;
uniform int mapping_mode;



uniform vec3 objectColor;

uniform float maxYval;
uniform float minYval;

uniform vec3 k_a;
uniform vec3 k_d;
uniform vec3 k_s;
uniform float ns;
uniform vec3 emissive;

vec3 n_normal=normalize(Normal);
vec2 realTexCoord;
vec3 I_a,I_d,I_s;

struct Light
{
	uint type;    	   //            0
	vec3 direction;    // D,S        16
	vec3 position;     // P          32

	vec3 ambient;      // D,P,S      48
	vec3 diffuse;      // D,P,S      64
	vec3 specular;     // D,P,S      80
	vec3 c;
	float inner_angle; // S          96
	float outer_angle; // S          112
	float falloff;     // S          128
};

layout(std140, binding = 1) uniform LightInformation
{	
	uint light_number;
	float near;
	float far;	
	vec3 view_position;
	vec3 fog_color;
	vec3 global_ambient_color;
	Light lights[16];
};

vec3 CalculateLight(Light light)
{	

	I_a= light.ambient*k_a;

		vec3 light_vector = normalize(light.position-FragPosition);


			I_d = k_d*light.diffuse*max(dot(n_normal,light_vector),0.0);

		vec3 view_vector=normalize(view_position-FragPosition);
		vec3 reflectDirection = 2*dot(n_normal,light_vector)*n_normal-light_vector;

		if(dot(n_normal,light_vector)>0.0)
		{

			I_s = k_s*light.specular*pow(max(dot(view_vector,reflectDirection),0.0),32)/3; 
		}
		else
			I_s=vec3(0,0,0);
		
		float light_length=length(light.position-FragPosition);
		float attenuation=min(1/(light.c.x+light.c.y*light_length+light.c.z*light_length*light_length),1);
		vec3 I_local = attenuation*(I_a+I_d+I_s);
		return I_local;

}

void main()
{
	if(should_use_gpuside_uv)
	{
		vec3 tmpVec3,abstmpVec3;
		float u,v;
		if(mapping_with_normal)
			tmpVec3=normalize(norm);
		else
			tmpVec3=pos;
		abstmpVec3=abs(tmpVec3);
		switch(mapping_mode)
		{
			case 0:
				realTexCoord=TexCoord;
				break;
			case 1:
			    u = degrees(atan(tmpVec3.z , tmpVec3.x));
    			u+=180;
   				v = 180-(acos(tmpVec3.y / (sqrt(tmpVec3.x * tmpVec3.x + tmpVec3.y * tmpVec3.y + tmpVec3.z * tmpVec3.z))) * 180.f / acos(-1)) ;
           
				realTexCoord = vec2(u / 360.f, v / 180.f);
				break;
			case 2:
			    u = degrees(atan(tmpVec3.z , tmpVec3.x));
				u+=180;
   				v = (tmpVec3.y-minYval)/(maxYval-minYval);
   				realTexCoord = vec2(u/360.f,v);
   				break;
   			case 3:
           		if (abstmpVec3.x >= abstmpVec3.y && abstmpVec3.x >= abstmpVec3.z)
            	{
                	if(tmpVec3.x < 0)
                		realTexCoord.x = tmpVec3.z/abstmpVec3.x;
                	else
                		realTexCoord.x = -tmpVec3.z/abstmpVec3.x;
                	realTexCoord.y = tmpVec3.y/abstmpVec3.x;
	            }
	            if (abstmpVec3.y >= abstmpVec3.x && abstmpVec3.y >= abstmpVec3.z)
	            {
	                if(tmpVec3.y < 0)
	                	realTexCoord.x = tmpVec3.x/abstmpVec3.y;
	                else
	                	realTexCoord.x = -tmpVec3.x/abstmpVec3.y;
	                realTexCoord.y = tmpVec3.z/abstmpVec3.y;
	            }
	            if (abstmpVec3.z >= abstmpVec3.y && abstmpVec3.z >= abstmpVec3.x)
	            {
	                if(tmpVec3.z < 0) 
	                	realTexCoord.x = -tmpVec3.x/abstmpVec3.z;
	                else
	                	realTexCoord.x = tmpVec3.x/abstmpVec3.z;
	                realTexCoord.y = tmpVec3.y/abstmpVec3.z;
	            }
	            realTexCoord=(realTexCoord+vec2(1))/2;
   				break;
   			default:
   				break;
		}
	}
	else
		realTexCoord=TexCoord;

	vec3 result=vec3(0);
	for(int i=0;i<light_number;i++)
	{
		result+=CalculateLight(lights[i]);
	}

	
	result+=global_ambient_color*k_a+emissive;;
	float fog_factor=(far-length(view_position-FragPosition))/(far-near);
	result =  fog_factor*result+(1-fog_factor)*fog_color;
	result=vec3(min(result.x,1),min(result.y,1),min(result.z,1));
	if(item_selected)
	{
		FragColor = vec4(result*vec3(1,0.3,0.3),1.0);		
	}
	else
	{

		FragColor = vec4(result*objectColor,1.0);

	}
}