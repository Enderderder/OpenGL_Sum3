#pragma once


// https://realitymultiplied.wordpress.com/2016/07/23/assimp-skeletal-animation-tutorial-2-loading-up-the-bone-data/
// https://github.com/SaschaWillems/Vulkan/blob/master/skeletalanimation/skeletalanimation.cpp
// http://techqa.info/programming/tag/assimp?after=38129536
// https://github.com/akafonzie/OpenGLAssimpModelLoader/blob/master/modelLoader.cpp

//aiMath to glm conversion
// https://github.com/mackron/GTGameEngine/blob/master/source/ModelDefinition_Assimp.cpp

#include "Utility.h"

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"

#include "Dependencies/assimp/Importer.hpp"
#include "Dependencies/assimp/scene.h"
#include "Dependencies/assimp/postprocess.h"

// Forward Declare
class CCamera;
class CTerrainComponent;

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ZERO_MEM_VAR(var) memset(&var, 0, sizeof(var))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)
#define GLCheckError() (glGetError() == GL_NO_ERROR)

class ssAnimatedModel
{
public:
	ssAnimatedModel(
		std::string modelFilname,
		std::string texFilename,
		CCamera*_camera,
		GLuint _program);
	~ssAnimatedModel();

	bool loadMesh(std::string fileName);
	void render(float _deltaTime);

	GLuint numBones() {
		return mNumBones;
	}

	void boneTransforms(float timeInSeconds, std::vector<Matrix4f> &transforms);
	void setCurrentAnimation(int startFrameNum, int endFramNum);

	void setPosition(glm::vec3 _position);
	void setRotation(glm::vec3 _rotation);
	void setScale(glm::vec3 _scale);
	void setSpeed(float _speed);

	void move(float speed);
	void rotate(float speed);
	
	bool bMoving = false;

private:

	CCamera* m_camera;
	GLuint m_program;

	//GLuint textureID;

int mNumBones;

#define NUM_BONES_PER_VERTEX 4

	struct BoneInfo {

		Matrix4f BoneOffset;
		Matrix4f FinalTransformation;

		BoneInfo() {
			BoneOffset.SetZero();// = glm::mat4(); //-->setzero
			FinalTransformation.SetZero();  // = glm::mat4();  //--> setzero
		}
	};

	struct VertexBoneData {

		int IDs[NUM_BONES_PER_VERTEX];
		float Weights[NUM_BONES_PER_VERTEX];

		VertexBoneData() {
			reset();
		}

		void reset() {
			ZERO_MEM(IDs);
			ZERO_MEM(Weights);
		}

		int boneID; float weight;
		void addBoneData(int BoneID, float Weight);
	};

	bool InitFromScene(const aiScene* pScene, const std::string Filename);

	void initMesh(GLuint meshIndex,
		const aiMesh*paiMesh,
		std::vector<glm::vec3>& positions,
		std::vector<glm::vec3>& normals,
		std::vector<glm::vec2>&texcoords,
		std::vector<VertexBoneData>&bones,
		std::vector<GLuint>&indices);

	void loadBones(int meshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& bones);
	bool initMaterials(const aiScene* pScene, const std::string filename);
	GLuint loadTexture(std::string  texFileName);
	void Clear();

	//data conversion and finder helper functions 
	
	GLuint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	GLuint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	GLuint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

	void CalcInterpolatedPosition(aiVector3D& out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedScaling(aiVector3D& out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	
	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, std::string NodeName);
	
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform);

	void setShaderEffectVariables(float dt);

	//rendering

	#define INVALID_MATERIAL 0xFFFFFFFF

	enum VB_TYPES {
		INDEX_BUFFER,
		POS_VB,
		NORMAL_VB,
		TEXCOORD_VB,
		BONE_VB,
		NUM_VBs
	};

	struct MeshEntry {

		MeshEntry() {
		
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = INVALID_MATERIAL;
		
		}

		GLuint NumIndices;
		GLuint BaseVertex;
		GLuint BaseIndex;
		GLuint MaterialIndex;
	};

	GLuint m_vao;
	GLuint m_buffers[NUM_VBs];
	
	std::vector<MeshEntry> m_Entries;
	std::vector<GLuint> m_Textures;

	std::map<std::string, GLuint> m_BoneMapping;
	std::map<std::string, aiNodeAnim*> boneHierarchy;
	std::vector<BoneInfo> m_BoneInfo;
	GLuint m_NumBones;

	Matrix4f m_GlobalInverseTransform;
	GLuint m_boneLocation[100];

	const aiScene* m_pScene;
	Assimp::Importer m_Importer;

	GLuint numMaterialsCounter;

	float animTick;

	int startFrame;
	int endFrame;
	int currentFrame;
	float animSpeed;

	int animFps;
	float animStartTime;
	float animEndtime;
	
	float animationTime;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	float currentPlayerSpeed;
	float currentRotationSpeed;

};

