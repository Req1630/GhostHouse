xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}


Material lambert6 {
 1.000000;1.000000;1.000000;1.000000;;
 0.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "doa_swithtex_Red.png";
 }
}

Frame doa_swich {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Frame polySurface3 {
  

  FrameTransformMatrix {
   1.000000,0.000000,0.000000,0.000000,0.000000,-0.000000,-1.000000,0.000000,0.000000,1.000000,-0.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
  }

  Mesh polySurface3 {
   62;
   -17.619213;-1.942535;-0.001671;,
   17.663880;-1.942538;65.816710;,
   17.663883;-1.942536;-0.001673;,
   -17.619213;-1.942535;65.816710;,
   -17.619213;-1.942535;65.816710;,
   17.663886;-2.748837;65.816710;,
   17.663880;-1.942538;65.816710;,
   -17.619213;-2.748833;65.816710;,
   -17.619213;-2.748833;65.816710;,
   17.663880;-2.748836;-0.001671;,
   17.663886;-2.748837;65.816710;,
   -17.619211;-2.748828;-0.001675;,
   17.663883;-1.942536;-0.001673;,
   17.663886;-2.748837;65.816710;,
   17.663880;-2.748836;-0.001671;,
   17.663880;-1.942538;65.816710;,
   -17.619211;-2.748828;-0.001675;,
   -17.619213;-1.942535;65.816710;,
   -17.619213;-1.942535;-0.001671;,
   -17.619213;-2.748833;65.816710;,
   0.022335;0.000008;65.816710;,
   25.022334;0.000008;100.000000;,
   17.663883;0.000008;65.816710;,
   0.022335;0.000011;100.000000;,
   0.022335;-4.999996;100.000000;,
   17.663883;-4.999996;65.816710;,
   25.022334;-4.999996;100.000000;,
   0.022335;-4.999996;65.816710;,
   25.022334;0.000000;0.000000;,
   17.663883;0.000000;-0.001672;,
   17.663883;-5.000000;-0.001671;,
   25.022334;-5.000000;0.000000;,
   0.022335;0.000011;100.000000;,
   25.022334;-4.999996;100.000000;,
   25.022334;0.000008;100.000000;,
   0.022335;-4.999996;100.000000;,
   17.663883;0.000008;65.816710;,
   0.022335;-4.999996;65.816710;,
   0.022335;0.000008;65.816710;,
   17.663883;-4.999996;65.816710;,
   17.663883;0.000008;65.816710;,
   17.663883;-5.000000;-0.001671;,
   17.663883;-4.999996;65.816710;,
   17.663883;0.000000;-0.001672;,
   -24.977666;0.000008;100.000000;,
   -17.619213;0.000008;65.816710;,
   -17.619213;-4.999996;65.816710;,
   -24.977666;-4.999996;100.000000;,
   -24.977666;0.000000;0.000000;,
   -17.619213;0.000000;-0.001672;,
   -17.619213;-5.000000;-0.001671;,
   -24.977666;-5.000000;0.000000;,
   0.022335;0.000011;100.000000;,
   -24.977666;-4.999996;100.000000;,
   0.022335;-4.999996;100.000000;,
   -24.977666;0.000008;100.000000;,
   -17.619213;0.000008;65.816710;,
   -17.619213;-4.999996;65.816710;,
   -17.619213;0.000008;65.816710;,
   -17.619213;-5.000000;-0.001671;,
   -17.619213;0.000000;-0.001672;,
   -17.619213;-4.999996;65.816710;;
   38;
   3;0,1,2;,
   3;1,0,3;,
   3;4,5,6;,
   3;5,4,7;,
   3;8,9,10;,
   3;9,8,11;,
   3;12,13,14;,
   3;13,12,15;,
   3;16,17,18;,
   3;17,16,19;,
   3;20,21,22;,
   3;21,20,23;,
   3;24,25,26;,
   3;25,24,27;,
   3;22,28,29;,
   3;28,22,21;,
   3;30,26,25;,
   3;26,30,31;,
   3;32,33,34;,
   3;33,32,35;,
   3;36,37,38;,
   3;37,36,39;,
   3;40,41,42;,
   3;41,40,43;,
   3;20,44,23;,
   3;44,20,45;,
   3;24,46,27;,
   3;46,24,47;,
   3;45,48,44;,
   3;48,45,49;,
   3;50,47,51;,
   3;47,50,46;,
   3;52,53,54;,
   3;53,52,55;,
   3;56,37,57;,
   3;37,56,38;,
   3;58,59,60;,
   3;59,58,61;;

   MeshNormals {
    62;
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;1.000000;0.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    -0.000000;-1.000000;-0.000000;,
    -0.000000;-1.000000;-0.000000;,
    -0.000000;-1.000000;-0.000000;,
    -0.000000;-1.000000;-0.000000;,
    1.000000;0.000002;-0.000000;,
    1.000000;0.000002;-0.000000;,
    1.000000;0.000002;-0.000000;,
    1.000000;0.000002;-0.000000;,
    -1.000000;-0.000001;0.000000;,
    -1.000000;-0.000001;0.000000;,
    -1.000000;-0.000001;0.000000;,
    -1.000000;-0.000001;0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;-1.000000;0.000000;,
    -0.000000;-1.000000;0.000000;,
    -0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;1.000000;-0.000000;,
    0.000000;1.000000;-0.000000;,
    -0.000000;-1.000000;0.000000;,
    -0.000000;-1.000000;0.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -1.000000;0.000000;0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    -0.000000;1.000000;-0.000000;,
    -0.000000;1.000000;-0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;-1.000000;0.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;1.000000;,
    0.000000;0.000000;-1.000000;,
    0.000000;0.000000;-1.000000;,
    1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;,
    1.000000;0.000000;-0.000000;;
    38;
    3;0,1,2;,
    3;1,0,3;,
    3;4,5,6;,
    3;5,4,7;,
    3;8,9,10;,
    3;9,8,11;,
    3;12,13,14;,
    3;13,12,15;,
    3;16,17,18;,
    3;17,16,19;,
    3;20,21,22;,
    3;21,20,23;,
    3;24,25,26;,
    3;25,24,27;,
    3;22,28,29;,
    3;28,22,21;,
    3;30,26,25;,
    3;26,30,31;,
    3;32,33,34;,
    3;33,32,35;,
    3;36,37,38;,
    3;37,36,39;,
    3;40,41,42;,
    3;41,40,43;,
    3;20,44,23;,
    3;44,20,45;,
    3;24,46,27;,
    3;46,24,47;,
    3;45,48,44;,
    3;48,45,49;,
    3;50,47,51;,
    3;47,50,46;,
    3;52,53,54;,
    3;53,52,55;,
    3;56,37,57;,
    3;37,56,38;,
    3;58,59,60;,
    3;59,58,61;;
   }

   MeshMaterialList {
    1;
    38;
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0;
    { lambert6 }
   }

   MeshTextureCoords c1 {
    62;
    0.014576;0.998685;,
    0.390862;0.358220;,
    0.390862;0.998685;,
    0.014576;0.358220;,
    0.570391;0.075693;,
    0.890376;0.029689;,
    0.890376;0.075693;,
    0.570390;0.029689;,
    0.014576;0.358220;,
    0.390862;0.998685;,
    0.390862;0.358220;,
    0.014576;0.998685;,
    0.923740;0.941060;,
    0.991054;0.058940;,
    0.991053;0.941060;,
    0.923739;0.058940;,
    0.923740;0.941060;,
    0.991054;0.058940;,
    0.991053;0.941060;,
    0.923739;0.058940;,
    0.171798;0.341827;,
    0.535986;0.000000;,
    0.426384;0.341827;,
    0.171798;0.000000;,
    0.171798;0.000000;,
    0.426384;0.341827;,
    0.535986;0.000000;,
    0.171798;0.341827;,
    0.535986;0.999983;,
    0.426384;1.000000;,
    0.426384;1.000000;,
    0.535986;0.999983;,
    0.604941;0.007637;,
    0.846135;0.007637;,
    0.846135;0.007637;,
    0.604941;0.007637;,
    0.426384;0.341827;,
    0.171798;0.341827;,
    0.171798;0.341827;,
    0.426384;0.341827;,
    0.426384;0.341827;,
    0.426384;1.000000;,
    0.426384;0.341827;,
    0.426384;1.000000;,
    0.535986;0.000000;,
    0.426384;0.341827;,
    0.426384;0.341827;,
    0.535986;0.000000;,
    0.535986;0.999983;,
    0.426384;1.000000;,
    0.426384;1.000000;,
    0.535986;0.999983;,
    0.604941;0.002974;,
    0.846135;0.002974;,
    0.604941;0.002974;,
    0.846135;0.002974;,
    0.426384;0.341827;,
    0.426384;0.341827;,
    0.426384;0.341827;,
    0.426384;1.000000;,
    0.426384;1.000000;,
    0.426384;0.341827;;
   }
  }
 }
}