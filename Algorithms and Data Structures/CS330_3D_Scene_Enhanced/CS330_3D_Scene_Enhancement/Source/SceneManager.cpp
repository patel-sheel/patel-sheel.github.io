///////////////////////////////////////////////////////////////////////////////
// scenemanager.cpp
// ============
// manage the preparing and rendering of 3D scenes - textures, materials, lighting
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager *pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glGenTextures(1, &m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  FindTextureID()
 *
 *  This method is used for getting an ID for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_textureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  This method is used for getting a slot index for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureSlot = index;
			bFound = true;
		}
		else
			index++;
	}

	return(textureSlot);
}

/***********************************************************
 *  FindMaterial()
 *
 *  This method is used for getting a material from the previously
 *  defined materials list that is associated with the passed in tag.
 ***********************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.ambientColor = m_objectMaterials[index].ambientColor;
			material.ambientStrength = m_objectMaterials[index].ambientStrength;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(true);
}

/***********************************************************
 *  SetTransformations()
 *
 *  This method is used for setting the transform buffer
 *  using the passed in transformation values.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationZ * rotationY * rotationX * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}


/***********************************************************
 *  DefineObjectMaterials()
 *
 *  This method is used for configuring the various material
 *  settings for all of the objects within the 3D scene.
 ***********************************************************/
void SceneManager::DefineObjectMaterials()
{
	/*** STUDENTS - add the code BELOW for defining object materials. ***/
	/*** There is no limit to the number of object materials that can ***/
	/*** be defined. Refer to the code in the OpenGL Sample for help  ***/
	OBJECT_MATERIAL goldMaterial;
	goldMaterial.ambientColor = glm::vec3(0.2f, 0.2f, 0.1f);
	goldMaterial.ambientStrength = 0.4f;
	goldMaterial.diffuseColor = glm::vec3(0.3f, 0.3f, 0.2f);
	goldMaterial.specularColor = glm::vec3(0.6f, 0.5f, 0.4f);
	goldMaterial.shininess = 22.0;
	goldMaterial.tag = "gold";

	m_objectMaterials.push_back(goldMaterial);

	OBJECT_MATERIAL cementMaterial;
	cementMaterial.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	cementMaterial.ambientStrength = 0.2f;
	cementMaterial.diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
	cementMaterial.specularColor = glm::vec3(0.4f, 0.4f, 0.4f);
	cementMaterial.shininess = 0.5;
	cementMaterial.tag = "cement";

	m_objectMaterials.push_back(cementMaterial);

	OBJECT_MATERIAL woodMaterial;
	woodMaterial.ambientColor = glm::vec3(0.4f, 0.3f, 0.1f);
	woodMaterial.ambientStrength = 0.2f;
	woodMaterial.diffuseColor = glm::vec3(0.3f, 0.2f, 0.1f);
	woodMaterial.specularColor = glm::vec3(0.1f, 0.1f, 0.1f);
	woodMaterial.shininess = 0.3;
	woodMaterial.tag = "wood";

	m_objectMaterials.push_back(woodMaterial);

	OBJECT_MATERIAL tileMaterial;
	tileMaterial.ambientColor = glm::vec3(0.2f, 0.3f, 0.4f);
	tileMaterial.ambientStrength = 0.3f;
	tileMaterial.diffuseColor = glm::vec3(0.3f, 0.2f, 0.1f);
	tileMaterial.specularColor = glm::vec3(0.4f, 0.5f, 0.6f);
	tileMaterial.shininess = 25.0;
	tileMaterial.tag = "tile";

	m_objectMaterials.push_back(tileMaterial);

	OBJECT_MATERIAL glassMaterial;
	glassMaterial.ambientColor = glm::vec3(0.4f, 0.4f, 0.4f);
	glassMaterial.ambientStrength = 0.3f;
	glassMaterial.diffuseColor = glm::vec3(0.3f, 0.3f, 0.3f);
	glassMaterial.specularColor = glm::vec3(0.6f, 0.6f, 0.6f);
	glassMaterial.shininess = 85.0;
	glassMaterial.tag = "glass";

	m_objectMaterials.push_back(glassMaterial);

	OBJECT_MATERIAL clayMaterial;
	clayMaterial.ambientColor = glm::vec3(0.2f, 0.2f, 0.3f);
	clayMaterial.ambientStrength = 0.3f;
	clayMaterial.diffuseColor = glm::vec3(0.4f, 0.4f, 0.5f);
	clayMaterial.specularColor = glm::vec3(0.2f, 0.2f, 0.4f);
	clayMaterial.shininess = 0.5;
	clayMaterial.tag = "clay";

	m_objectMaterials.push_back(clayMaterial);

	OBJECT_MATERIAL metalMaterial;
	metalMaterial.diffuseColor = glm::vec3(0.4f, 0.4f, 0.4f);
	metalMaterial.specularColor = glm::vec3(0.6f, 0.6f, 0.6f);
	metalMaterial.shininess = 52.0;
	metalMaterial.tag = "metal";

	m_objectMaterials.push_back(metalMaterial);

}

/***********************************************************
 *  SetupSceneLights()
 *
 *  This method is called to add and configure the light
 *  sources for the 3D scene.  There are up to 4 light sources.
 ***********************************************************/
void SceneManager::SetupSceneLights()
{
	// this line of code is NEEDED for telling the shaders to render 
	// the 3D scene with custom lighting, if no light sources have
	// been added then the display window will be black - to use the 
	// default OpenGL lighting then comment out the following line
	//m_pShaderManager->setBoolValue(g_UseLightingName, true);

	/*** STUDENTS - add the code BELOW for setting up light sources ***/
	/*** Up to four light sources can be defined. Refer to the code ***/
	/*** in the OpenGL Sample for help                              ***/

	m_pShaderManager->setBoolValue(g_UseLightingName, true);
	m_pShaderManager->setVec3Value("globalAmbientColor", 0.3f, 0.3f, 0.3f);

	/*
	m_pShaderManager->setVec3Value("lightSources[0].position", -5.0f, 10.0f, 10.0f);
	m_pShaderManager->setVec3Value("lightSources[0].diffuseColor", 0.7f, 0.1f, 0.05f);
	m_pShaderManager->setVec3Value("lightSources[0].specularColor", .5f, 0.01f, 0.005f);
	m_pShaderManager->setFloatValue("lightSources[0].focalStrength", 18.0f);
	m_pShaderManager->setFloatValue("lightSources[0].specularIntensity", 0.35f);
	*/


	m_pShaderManager->setVec3Value("lightSources[0].position", 5.0f, 15.0f, 6.0f);
	m_pShaderManager->setVec3Value("lightSources[0].diffuseColor", 0.4f, 0.4f, 0.4f);
	m_pShaderManager->setVec3Value("lightSources[0].specularColor", 0.25f, 0.25f, 0.25f);
	m_pShaderManager->setFloatValue("lightSources[0].focalStrength", 16.0f);
	m_pShaderManager->setFloatValue("lightSources[0].specularIntensity", 0.1f);


	m_pShaderManager->setVec3Value("lightSources[1].position", 10.0f, -5.0f, -3.0f);
	m_pShaderManager->setVec3Value("lightSources[1].diffuseColor", 0.4f, 0.4f, 0.4f);
	m_pShaderManager->setVec3Value("lightSources[1].specularColor", .25f, 0.25f, 0.25f);
	m_pShaderManager->setFloatValue("lightSources[1].focalStrength", 8.0f);
	m_pShaderManager->setFloatValue("lightSources[1].specularIntensity", 0.1f);


}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 ***********************************************************/
void SceneManager::SetShaderTexture(
	std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}

/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			m_pShaderManager->setVec3Value("material.ambientColor", material.ambientColor);
			m_pShaderManager->setFloatValue("material.ambientStrength", material.ambientStrength);
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}

/**************************************************************/
/*** STUDENTS CAN MODIFY the code in the methods BELOW for  ***/
/*** preparing and rendering their own 3D replicated scenes.***/
/*** Please refer to the code in the OpenGL sample project  ***/
/*** for assistance.                                        ***/
/**************************************************************/


/***********************************************************
 *  LoadSceneTextures()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene
 *  rendering
 ***********************************************************/
void SceneManager::LoadSceneTextures()
{
	bool bReturn = false;

	bReturn = CreateGLTexture(
		"textures/snow-ground_base_1k.jpg",
		"snow1");

	bReturn = CreateGLTexture(
		"textures/snow-ground_rough_1k.jpg",
		"snow2");

	bReturn = CreateGLTexture(
		"textures/green-grass-leaf_base_1k.jpg",
		"grass");

	bReturn = CreateGLTexture(
		"textures/grass-gravel_base_1k.jpg",
		"gravel");

	bReturn = CreateGLTexture(
		"textures/wood-oak-tree-bark_base_1k.jpg",
		"treetrunk");
	bReturn = CreateGLTexture(
		"textures/gold-seamless-texture.jpg",
		"gold_seamless"
	);
	bReturn = CreateGLTexture(
		"textures/circular-brushed-gold-texture.jpg",
		"gold_circular"
	);

	bReturn = CreateGLTexture(
		"textures/pavers.jpg",
		"pavers"
	);

	bReturn = CreateGLTexture(
		"textures/sky.jpg",
		"sky"
	);

	bReturn = CreateGLTexture(
		"textures/test.jpg",
		"egg"
	);

	bReturn = CreateGLTexture(
		"textures/grass2.jpg",
		"grass2"
	);
	bReturn = CreateGLTexture(
		"textures/mountain.jpg",
		"mountain"
	);

	bReturn = CreateGLTexture(
		"textures/platform2.jpg",
		"platform");

	// after the texture image data is loaded into memory, the
	// loaded textures need to be bound to texture slots - there
	// are a total of 16 available slots for scene textures
	BindGLTextures();
}

/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene 
 *  rendering
 ***********************************************************/
void SceneManager::PrepareScene()
{
	// load the texture image files for the textures applied
	// to objects in the 3D scene
	LoadSceneTextures();
	DefineObjectMaterials();
	SetupSceneLights();
	// only one instance of a particular mesh needs to be
	// loaded in memory no matter how many times it is drawn
	// in the rendered 3D scene

	m_basicMeshes->LoadPlaneMesh();
	m_basicMeshes->LoadSphereMesh();
	m_basicMeshes->LoadPyramid4Mesh();
	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadBoxMesh();
	m_basicMeshes->LoadTorusMesh();
	m_basicMeshes->LoadConeMesh();
	m_basicMeshes->LoadTaperedCylinderMesh();
	m_basicMeshes->LoadConeMesh();
	m_basicMeshes->LoadPyramid3Mesh();

}

/***********************************************************
 *  RenderScene()
 *
 *  This method is used for rendering the 3D scene by 
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderScene()
{
	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	//Base Plane

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(20.0f, 1.0f, 10.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.45, 0.58, 0.63, 1.0);
	SetShaderTexture("pavers");

	//TODO change back to cement before submitting final project.
	//Set to glass for Module 6 milestone
	SetShaderMaterial("cement");
	SetTextureUVScale(15.0, 15.0);
	// draw the mesh with transformation values
	m_basicMeshes->DrawPlaneMesh();
	/****************************************************************/

	//Back Plane

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(20.0f, 1.0f, 10.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 90.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.0f, 9.0f, -10.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.11, 0.77, 0.77, 1.0);
	SetShaderTexture("sky");
	SetTextureUVScale(1.0, 1.0);
	// draw the mesh with transformation values
	m_basicMeshes->DrawPlaneMesh();
	/****************************************************************/

	MakePlatform(-10.0f, 1.0f, 0.5f, 3.0f);
	MakePlatform(-4.0f, 1.0f, 0.5f, 3.0f);
	MakePlatform(2.0f, 3.5f, 0.5f, 3.0f);
	MakePlatform(8.0f, 2.0f, 0.5f, 6.0f);
	MakeCoin(12,3);
	MakeKey(-8.0f, 3.5f, 0.5f);
	MakeEgg(-8.0f, 2.8f, 0.5f);
	MakeEgg(-7.0f, 4.0f, 0.5f);
	MakeEgg(-5.4f, 5.4f, 0.5f);
	MakeEgg(-3.5f, 6.3f, 0.5f);
	MakeMountain(6.0f, 0.0f, -5.0f, 2.0f);
	MakeMountain(-12.0f, 0.0f, -5.0f, 0.0f);
	MakeTreeLeft(0.0f, 0.0f, 1.0f, 15.0f);
	MakeTreeRight(1.0f, 0.0f, 1.0f, -15.0f);
	MakeTreeCenter(18.4f, 2.1f, 1.0f);	


}
// All off the classes below were created to keep the shapes needed to create each item in a single function which could be used repeatedly and placed where needed.
// It also helped to reduce how much scrolling was necessary through the render scene function. It additionally cut down on the number of changes which would have
// had to be made if items used multiple times needed small edits.

// Coin creation so we dont have to keep making individual coins
void SceneManager::MakeCoin(int col, int row) {

	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;


	for (int j = 0; j < row; ++j) {
		// for loop to create coins across
		for (int i = 0; i < col; ++i) {
			if ((i + j) % 2 == 0) {
				continue;
			}
			scaleXYZ = glm::vec3(0.3f, 0.1f, 0.2f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 90.0f;
			YrotationDegrees = 135.0f;
			ZrotationDegrees = 0.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3((-10.0f + (i * 2)), (8.0f + (j * 2)), 0.0f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);

			//SetShaderColor(1, 1, 1, 1);
			SetShaderTexture("gold_seamless");
			SetTextureUVScale(1.0, 1.0);
			SetShaderMaterial("gold");

			// draw the mesh with transformation values - this plane is used for the base
			m_basicMeshes->DrawCylinderMesh();
		}
	}
}

// Tree creation for a Tree tilted to the left as branches have to be slight offset
void SceneManager::MakeTreeLeft(float xloc, float yloc, float zloc, float zrot) {


	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = zrot;
	glm::vec3 positionXYZ;


/****************************************************************/

//Leftmost tree trunk for leftmost mountain

/*** Set needed transformations before drawing the basic mesh.  ***/
/*** This same ordering of code should be used for transforming ***/
/*** and drawing all the basic 3D shapes.						***/
/******************************************************************/
// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.1f, 0.4f, 0.1f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = zrot;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((-12.5f + xloc), (6.1f + yloc), (-6.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.46, 0.36, 0.16, 1.0);
	SetShaderTexture("treetrunk");
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();

	/****************************************************************/

	//Leftmost tree branch base for leftmost mountain

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.35f, 0.35f, 0.35f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = zrot;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((-12.64f + xloc), (6.55f + yloc), (-6.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.14, 0.26, 0.07, 1.0);
	SetShaderTexture("grass");
	SetShaderMaterial("wood");
	SetTextureUVScale(1.0, 1.0);
	// draw the mesh with transformation values
	m_basicMeshes->DrawPyramid4Mesh();

	/****************************************************************/

	//Leftmost tree branch center for leftmost mountain

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.35f, 0.35f, 0.35f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = zrot;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((-12.68 + xloc), (6.68f + yloc), (-6.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.14, 0.26, 0.07, 1.0);
	SetShaderTexture("grass");
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPyramid4Mesh();



	/****************************************************************/

	//Leftmost tree branch top for leftmost mountain

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.35f, 0.35f, 0.35f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = zrot;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((-12.7f + xloc), (6.8f + yloc), (-6.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.14, 0.26, 0.07, 1.0);
	SetShaderTexture("grass");
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPyramid4Mesh();

}

// Tree creation for a Tree tilted to the right as branches have to be slight offset
void SceneManager::MakeTreeRight(float xloc, float yloc, float zloc, float zrot) {


	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = zrot;
	glm::vec3 positionXYZ;


	/****************************************************************/

	//Leftmost tree trunk for leftmost mountain

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.1f, 0.4f, 0.1f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = zrot;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((-12.5f + xloc), (6.1f + yloc), (-6.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.46, 0.36, 0.16, 1.0);
	SetShaderTexture("treetrunk");
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();

	/****************************************************************/

	//Leftmost tree branch base for leftmost mountain

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.35f, 0.35f, 0.35f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = zrot;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((-12.36f + xloc), (6.55f + yloc), (-6.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.14, 0.26, 0.07, 1.0);
	SetShaderTexture("grass");
	SetShaderMaterial("wood");
	SetTextureUVScale(1.0, 1.0);
	// draw the mesh with transformation values
	m_basicMeshes->DrawPyramid4Mesh();

	/****************************************************************/

	//Leftmost tree branch center for leftmost mountain

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.35f, 0.35f, 0.35f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = zrot;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((-12.34 + xloc), (6.68f + yloc), (-6.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.14, 0.26, 0.07, 1.0);
	SetShaderTexture("grass");
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPyramid4Mesh();



	/****************************************************************/

	//Leftmost tree branch top for leftmost mountain

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.35f, 0.35f, 0.35f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = zrot;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((-12.30f + xloc), (6.8f + yloc), (-6.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.14, 0.26, 0.07, 1.0);
	SetShaderTexture("grass");
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPyramid4Mesh();

}

// Tree creation for a Tree going straight up as it needs no offset
void SceneManager::MakeTreeCenter(float xloc, float yloc, float zloc) {


	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;


	/****************************************************************/

	//Leftmost tree trunk for leftmost mountain

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.1f, 0.4f, 0.1f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((-12.7f + xloc), (6.1f + yloc), (-6.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.46, 0.36, 0.16, 1.0);
	SetShaderTexture("treetrunk");
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();

	/****************************************************************/

	//Leftmost tree branch base for leftmost mountain

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.35f, 0.35f, 0.35f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((-12.7f + xloc), (6.55f + yloc), (-6.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.14, 0.26, 0.07, 1.0);
	SetShaderTexture("grass");
	SetShaderMaterial("wood");
	SetTextureUVScale(1.0, 1.0);
	// draw the mesh with transformation values
	m_basicMeshes->DrawPyramid4Mesh();

	/****************************************************************/

	//Leftmost tree branch center for leftmost mountain

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.35f, 0.35f, 0.35f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((-12.7 + xloc), (6.68f + yloc), (-6.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.14, 0.26, 0.07, 1.0);
	SetShaderTexture("grass");
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPyramid4Mesh();



	/****************************************************************/

	//Leftmost tree branch top for leftmost mountain

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.35f, 0.35f, 0.35f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((-12.7f + xloc), (6.8f + yloc), (-6.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.14, 0.26, 0.07, 1.0);
	SetShaderTexture("grass");
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPyramid4Mesh();

}

// Tree creation for a Tree going straight up as it needs no offset
void SceneManager::MakeMountain(float xloc, float yloc, float zloc, float height) {


	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

/****************************************************************/

//Leftmost Mountain Cylinder

/*** Set needed transformations before drawing the basic mesh.  ***/
/*** This same ordering of code should be used for transforming ***/
/*** and drawing all the basic 3D shapes.						***/
/******************************************************************/
// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(2.0f, 5.0f + height, 2.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((0.0f + xloc), (0.0f + yloc), (0.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.46, 0.72, 0.46, 1.0);
	SetShaderTexture("mountain");
	SetTextureUVScale(20.0, 20.0);
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();

	/****************************************************************/

	//Leftmost Mountain Sphere

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(2.0f, 1.2f, 2.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((0.0f + xloc), (5.0f + yloc + height), (0.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.46, 0.72, 0.46, 1.0);
	SetShaderTexture("mountain");
	SetTextureUVScale(20.0, 20.0);
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawSphereMesh();

}

// Egg creation so we can reduce repetitive code
void SceneManager::MakeEgg(float xloc, float yloc, float zloc) {

	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	/******************************************************************/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.3f, 0.4f, 0.3f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((0.0f + xloc), (0.0f + yloc), (0.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(1.0, 1.0, 1.0, 1.0);

	SetShaderTexture("egg");
	SetTextureUVScale(1.0f, 1.0f);
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawSphereMesh();
}

// Key Creation out side of scene render to reduce clutter
void SceneManager::MakeKey(float xloc, float yloc, float zloc) {

	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;


	//Key Torus

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.25f, 0.15f, 0.5f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((0.0f + xloc), (0.65f + yloc), (0.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.9, 0.7, 0.4, 1.0);
	SetShaderTexture("gold_seamless");
	SetShaderMaterial("gold");

	SetTextureUVScale(1.0, 1.0);
	// draw the mesh with transformation values
	m_basicMeshes->DrawTorusMesh();

	/****************************************************************/

	//Key Shaft

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.1f, 0.5f, 0.1f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((0.0f + xloc), (0.0f + yloc), (0.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.9, 0.7, 0.4, 1.0);
	SetShaderTexture("gold_seamless");
	SetShaderMaterial("gold");
	SetTextureUVScale(1.0, 1.0);
	m_basicMeshes->DrawCylinderMesh(false, false, true);


	SetShaderTexture("gold_circular");
	SetShaderMaterial("gold");
	SetTextureUVScale(1.0, 1.0);
	m_basicMeshes->DrawCylinderMesh(true, true, false);
	SetShaderMaterial("gold");

	/****************************************************************/

	//Key Tooth 1

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.15f, 0.15f, 0.1f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((0.15f + xloc), (0.275f + yloc), (0.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.9, 0.7, 0.4, 1.0);
	SetShaderTexture("gold_seamless");
	SetShaderMaterial("gold");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();

	/****************************************************************/

	//Key Tooth 2

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(0.15f, 0.15f, 0.1f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3((0.15f + xloc), (0.1f + yloc), (0.0f + zloc));

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0.9, 0.7, 0.4, 1.0);
	SetShaderTexture("gold_seamless");
	SetShaderMaterial("gold");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();


	/****************************************************************/
}

// Platform creation to reduce clutter and make it easy to create platforms on demand and make tweaks en masse
void SceneManager::MakePlatform(float xloc, float yloc, float zloc, float height) {

	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(6.0f, height, 1.5f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(xloc, yloc , zloc);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(1, 1, 1, 1);
	SetShaderTexture("snow1");
	SetTextureUVScale(1.0, 1.0);
	SetShaderMaterial("cement");

	// draw the mesh with transformation values - this plane is used for the base
	m_basicMeshes->DrawBoxMesh();
}