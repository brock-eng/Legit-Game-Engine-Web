attribute highp vec4 position;
attribute highp vec2 texCoords;
attribute highp vec4 color;
attribute highp float textureID;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix;
uniform mat4 ml_matrix;


varying highp vec4 vs_position;
varying highp vec2 vs_texCoords;
varying highp vec4 vs_color;
varying highp float vs_textureID;

void main()
{
   vec4 calc_position = pr_matrix * position;
   gl_Position = calc_position;
   vs_position = calc_position;
   vs_texCoords = texCoords;
   vs_color = color;
   vs_textureID = textureID;
};

/* --------------------------------------------------------------- */


precision highp float;

varying highp vec4  vs_position;
varying highp vec2  vs_texCoords;
varying highp vec4  vs_color;
varying highp float vs_textureID;

uniform highp vec2 light_pos;

uniform sampler2D texture_0;
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;
uniform sampler2D texture_4;
uniform sampler2D texture_5;
uniform sampler2D texture_6;
uniform sampler2D texture_7;


void main()
{
   float intensity = 1.0 / length(vs_position.xy - light_pos);
   int textureCurr = int(fs_in.textureID);
   float noTexture = 0.0f;
   if (fs_in.textureID == noTexture)
      color = fs_in.color; // *intensity* light_level;
   else
   {
      if (vs_textureID == 1)
         color = texture2D(texture_0, vs_texCoords);
      else if (vs_textureID == 2)
         color = texture2D(texture_1, vs_texCoords);
      else if (vs_textureID == 3)
         color = texture2D(texture_2, vs_texCoords);
      else if (vs_textureID == 4)
         color = texture2D(texture_3, vs_texCoords);
      else if (vs_textureID == 5)
         color = texture2D(texture_4, vs_texCoords);
      else if (vs_textureID == 6)
         color = texture2D(texture_5, vs_texCoords);
      else if (vs_textureID == 7)
         color = texture2D(texture_6, vs_texCoords);
      else if (vs_textureID == 8)
         color = texture2D(texture_7, vs_texCoords);
      if (color == 0.0f) 
         discard;
      else
         color = color * intensity * light_level;
   }
};

