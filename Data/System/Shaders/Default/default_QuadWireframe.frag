#version 430 core

#define MAXCLIP 32

uniform vec4 meshColor;
float thickness = 0.005;

uniform float uDepthScale = 1.0f;
uniform int kindOIT; // 0 : None, 1 : WeightedBlended, 2 : DepthPeeling Init, 3 : DepthPeeling Peel.
uniform sampler2DRect depthPeelingDepthTex;


struct NodeType {
  vec4 color;
  float depth;
  uint next;
};

layout( binding = 0, r32ui) uniform uimage2D headPointers;
layout( binding = 0, offset = 0) uniform atomic_uint nextNodeCounter;
layout( binding = 0, std430 ) buffer BufferObject {
  NodeType nodes[];
};
uniform int MaxNodes;


in vec3 vBarycentric;
in vec4 vModelPos;
layout(location=0) out vec4 oColor;
layout(location=1) out vec4 oSumColor;
layout(location=2) out vec4 oSumWeight;



uniform int nClipPlanes;
uniform vec4 ClipPlanes[MAXCLIP];
uniform bool isProcess[MAXCLIP];

uniform bool hideBackfacing;
uniform bool swapUpAxes;

bool  isClip(vec4 worldPos)
{
    for(int i = 0; i < nClipPlanes; i++)
    {
        if(ClipPlanes[i].x*worldPos.x + ClipPlanes[i].y*worldPos.y + ClipPlanes[i].z*worldPos.z + ClipPlanes[i].w*worldPos.w < 0 && isProcess[i])
            return true;    
    }
    return false;
}




void writeWeightedBlendedOITAccumulateBuffer(vec4 color, float fragCoordW, out vec4 sumColor, out vec4 sumWeight)
{
	// Assuming that the projection matrix is a perspective projection
    // gl_FragCoord.w returns the inverse of the oPos.w register from the vertex shader
    float viewDepth = abs(1.0 / fragCoordW);

    // Tuned to work well with FP16 accumulation buffers and 0.001 < linearDepth < 2.5
    // See Equation (9) from http://jcgt.org/published/0002/02/09/
    float linearDepth = viewDepth * uDepthScale;
    float weight = clamp(0.03 / (1e-5 + pow(linearDepth, 4.0)), 1e-2, 3e3);

	sumColor = vec4(color.rgb * color.a, color.a) * weight;
	sumWeight = vec4(color.a);
}

// This is like
float aastep (float threshold, float dist) {
  float afwidth = fwidth(dist) * 0.5;
  return smoothstep(threshold - afwidth, threshold + afwidth, dist);
}

// This function is not currently used, but it can be useful
// to achieve a fixed width wireframe regardless of z-depth
float computeScreenSpaceWireframe (vec3 barycentric, float lineWidth) {
  vec3 dist = fwidth(barycentric);
  vec3 smoothed = smoothstep(dist * ((lineWidth * 0.5) - 0.5), dist * ((lineWidth * 0.5) + 0.5), barycentric);
  return 1.0 - min(min(smoothed.x, smoothed.y), smoothed.z);
}



// This function returns the fragment color for our styled wireframe effect
// based on the barycentric coordinates for this fragment
vec4 getStyledWireframe (vec3 barycentric) {
  // this will be our signed distance for the wireframe edge
  float d = min(min(barycentric.x, barycentric.y), barycentric.z);
  
  // the thickness of the stroke
  float computedThickness = thickness;

  // compute the anti-aliased stroke edge  
  float edge = 1.0 - aastep(computedThickness, d);
  
  if( edge <= 0.002)
	  discard;

  // now compute the final color of the mesh
  vec4 outColor = vec4(0.0);
  outColor = vec4(meshColor.rgb, edge);
  // if (!gl_FrontFacing) {
	// outColor.rgb = vec3(1, 0, 0);
  // }
  return outColor;
}

float evalMinDistanceToEdges(vec3 heights)
{
    float dist;

	vec3 ddxHeights = dFdx( heights );
	vec3 ddyHeights = dFdy( heights );
	vec3 ddHeights2 =  ddxHeights*ddxHeights + ddyHeights*ddyHeights;
	
    vec3 pixHeights2 = heights *  heights / ddHeights2 ; 
    
    dist = sqrt( min ( min (pixHeights2.x, pixHeights2.y), pixHeights2.z) );
    
    return dist;
}


void main()
{
	if(isClip(vModelPos))
        discard;
	
	if( hideBackfacing == true)
	{
		if(swapUpAxes)
		{
			if( gl_FrontFacing )
				discard;
		}
		else
		{
			if( !gl_FrontFacing )
				discard;
		}
	}
	
	float dist = evalMinDistanceToEdges(vBarycentric);

	// Cull fragments too far from the edge.
	if (dist > 1.0 ) discard;
	
	oColor = meshColor;
		
	//oColor = getStyledWireframe(vBarycentric);

	if(kindOIT == 1)
	{
		writeWeightedBlendedOITAccumulateBuffer(oColor, gl_FragCoord.w, oSumColor, oSumWeight);
	}
	else if(kindOIT == 2){
		oColor = vec4(oColor.rgb * oColor.a, 1.0 - oColor.a);
	}
	else if(kindOIT == 3){
		float frontDepth = texture(depthPeelingDepthTex, gl_FragCoord.xy).r;
		if(gl_FragCoord.z <= frontDepth)
			discard;
		oColor = vec4(oColor.rgb * oColor.a, oColor.a);
	}
    else if(kindOIT == 4) //Linked List
    {

        // Get the index of the next empty slot in the buffer
        uint nodeIdx = atomicCounterIncrement(nextNodeCounter);

        // Is our buffer full?  If so, we don't add the fragment
        // to the list.
        if( nodeIdx < MaxNodes ) {

            // Our fragment will be the new head of the linked list, so
            // replace the value at gl_FragCoord.xy with our new node's
            // index.  We use imageAtomicExchange to make sure that this
            // is an atomic operation.  The return value is the old head
            // of the list (the previous value), which will become the
            // next element in the list once our node is inserted.
            uint prevHead = imageAtomicExchange(headPointers, ivec2(gl_FragCoord.xy), nodeIdx);

            // Here we set the color and depth of this new node to the color
            // and depth of the fragment.  The next pointer, points to the
            // previous head of the list.
            nodes[nodeIdx].color = oColor;
            nodes[nodeIdx].depth = gl_FragCoord.z;
            nodes[nodeIdx].next = prevHead;

            
        }
    }
}