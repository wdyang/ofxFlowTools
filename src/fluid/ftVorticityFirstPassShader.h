
#pragma once

#include "ofMain.h"
#include "ftShader.h"

namespace flowTools {

	class ftVorticityFirstPassShader : public ftShader {
	public:
		ftVorticityFirstPassShader() {
			
			fragmentShader = STRINGIFY(uniform sampler2DRect Velocity;
									   uniform sampler2DRect Obstacle;
									   
									   void v2TexNeighbors(sampler2DRect tex, vec2 st,
														   out vec2 left, out vec2 right, out vec2 bottom, out vec2 top) {
										   left   = texture2DRect(tex, st - vec2(1, 0)).xy;
										   right  = texture2DRect(tex, st + vec2(1, 0)).xy;
										   bottom = texture2DRect(tex, st - vec2(0, 1)).xy;
										   top    = texture2DRect(tex, st + vec2(0, 1)).xy;
									   }
									   
									   void fRoundTexNeighbors(sampler2DRect tex, vec2 st,
															   out float left, out float right, out float bottom, out float top) {
										   left   = ceil(texture2DRect(tex, st - vec2(1, 0)).x - 0.5); // round not available
										   right  = ceil(texture2DRect(tex, st + vec2(1, 0)).x - 0.5);
										   bottom = ceil(texture2DRect(tex, st - vec2(0, 1)).x - 0.5);
										   top    = ceil(texture2DRect(tex, st + vec2(0, 1)).x - 0.5);
									   }
									   
									   void main(){
										   
										   vec2 st = gl_TexCoord[0].st;

										   vec2 vL; vec2 vR; vec2 vB; vec2 vT;
										   v2TexNeighbors (Velocity, st, vL, vR, vB, vT);
										   vec2 vC = texture2DRect(Velocity, st).xy;
										   
										   float oL; float oR; float oB; float oT;
										   fRoundTexNeighbors(Obstacle, st, oL, oR, oB, oT);
										   
										   vL = vL * (1.0 - oL) + vC * oL;
										   vR = vR * (1.0 - oR) + vC * oR;
										   vB = vB * (1.0 - oB) + vC * oB;
										   vT = vT * (1.0 - oT) + vC * oT;
										   
										   float vorticity = 0.5 * ((vR.y - vL.y) - (vT.x - vB.x));
										   gl_FragColor = vec4(vorticity, 0.0, 0.0, 0.0);
										   
									   }
									   );
			
			shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			shader.linkProgram();
		}
		
		void update(ofFbo& _buffer, ofTexture& _velocityTexture, ofTexture& _obstacleTexture){
			_buffer.begin();
			shader.begin();
			shader.setUniformTexture("Velocity", _velocityTexture, 0);
			shader.setUniformTexture("Obstacle", _obstacleTexture, 1);
			renderFrame(_buffer.getWidth(),_buffer.getHeight());
			shader.end();
			_buffer.end();
		}
	};
}