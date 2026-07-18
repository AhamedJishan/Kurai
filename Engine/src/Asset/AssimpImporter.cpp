#include "AssimpImporter.h"

#include <vector>
#include <unordered_map>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Utils/Log.h>
#include "RawModel.h"
#include "RawMesh.h"
#include "RawMaterial.h"
#include <Animation/Skeleton.h>
#include <Animation/Clip.h>

namespace Dawn:: AssimpImporter
{
	namespace
	{
		RawMesh* GetRawMesh(const aiMesh* aiMesh, Skeleton* skeleton);
		RawMaterial* GetRawMaterial(const aiMaterial* aiMat, const std::string& directory);
		Clip* GetAnimationClip(const aiAnimation* aiAnimation, const Skeleton* skeleton);
		Skeleton* GetSkeleton(const aiScene* aiScene);
	}


	RawModel* LoadRawModel(const std::string& filename)
	{
		const std::string directory = filename.substr(0, filename.find_last_of("/\\") + 1);

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filename,
			aiProcess_JoinIdenticalVertices |
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_LimitBoneWeights);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG_ERROR("Assimp error: %s", importer.GetErrorString());
			return nullptr;
		}

		RawModel* rawModel = new RawModel();
		rawModel->SetDirectory(directory);

		// Load Skeleton
		Skeleton* skeleton = GetSkeleton(scene);
		if (skeleton)
			rawModel->SetSkeleton(skeleton);

		// Load Animation Clips
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
			rawModel->AddAnimationClip(GetAnimationClip(scene->mAnimations[i], skeleton));

		// Load Meshes
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
			rawModel->AddRawMesh(GetRawMesh(scene->mMeshes[i], skeleton));

		// Load Materials
		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
			rawModel->AddRawMaterial(GetRawMaterial(scene->mMaterials[i], directory));

		return rawModel;
	}


	namespace
	{
		glm::mat4 AssimpToGlm(const aiMatrix4x4& m)
		{
			return glm::mat4(
				m.a1, m.b1, m.c1, m.d1,
				m.a2, m.b2, m.c2, m.d2,
				m.a3, m.b3, m.c3, m.d3,
				m.a4, m.b4, m.c4, m.d4
			);
		}
		glm::vec2 AssimpToGlm(const aiVector2D& v)
		{
			return glm::vec2(v.x, v.y);
		}
		glm::vec3 AssimpToGlm(const aiVector3D& v)
		{
			return glm::vec3(v.x, v.y, v.z);
		}
		glm::quat AssimpToGlm(const aiQuaternion& q)
		{
			return glm::quat(q.w, q.x, q.y, q.z);
		}

		RawMesh* GetRawMesh(const aiMesh* aiMesh, Skeleton* skeleton)
		{
			const std::string name = aiMesh->mName.C_Str();
			int materialIndex = aiMesh->mMaterialIndex;
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<VertexSkinData> skinDatas;

			// --- VERTICES ---
			vertices.reserve(aiMesh->mNumVertices);
			for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
			{
				Vertex vertex;

				vertex.Position = AssimpToGlm(aiMesh->mVertices[i]);
				vertex.Normal = AssimpToGlm(aiMesh->mNormals[i]);
				vertex.Tangent = AssimpToGlm(aiMesh->mTangents[i]);

				if (aiMesh->HasTextureCoords(0))
				{
					vertex.TexCoord = AssimpToGlm(aiMesh->mTextureCoords[0][i]);
				}

				vertices.emplace_back(vertex);
			}
			// --- INDICES ---
			indices.reserve(aiMesh->mNumFaces * 3);
			for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
			{
				const aiFace& aiFace = aiMesh->mFaces[i];
				for (unsigned int j = 0; j < aiFace.mNumIndices; j++)
				{
					indices.emplace_back(aiFace.mIndices[j]);
				}
			}
			// --- VERTEX SKIN DATA ---
			unsigned int numBones = aiMesh->mNumBones;
			bool hasSkinData = skeleton && numBones > 0;
			if (hasSkinData)
			{
				hasSkinData = true;
				skinDatas.resize(vertices.size());
				for (unsigned int aiBoneIndex = 0; aiBoneIndex < numBones; aiBoneIndex++)
				{
					const aiBone* bone = aiMesh->mBones[aiBoneIndex];

					std::string jointName = bone->mName.C_Str();
					int jointId = skeleton->GetJointId(jointName);

					unsigned int numWeights = bone->mNumWeights;
					for (unsigned int i = 0; i < numWeights; i++)
					{
						unsigned int vertexId = bone->mWeights[i].mVertexId;
						float weight = static_cast<float>( bone->mWeights[i].mWeight );

						if (vertexId >= vertices.size())
						{
							LOG_ERROR("Mesh '%s' vertexId '%d' is greater than vertex count '%d'", aiMesh->mName, vertexId, vertices.size());
							continue;
						}

						skinDatas[vertexId].AddWeight(jointId, weight);
					}
				}
			}

			return new RawMesh(name, materialIndex, std::move(vertices), std::move(indices), std::move(skinDatas), hasSkinData);
		}

		RawMaterial* GetRawMaterial(const aiMaterial* aiMat, const std::string& directory)
		{
			RawMaterial* rawMaterial = new RawMaterial();

			// Shininess
			float shininess;
			if (aiMat->Get(AI_MATKEY_SHININESS, shininess) == aiReturn_SUCCESS)
				rawMaterial->SetShininess(shininess);
			// Diffuse Color
			aiColor3D diffuseColor;
			if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == aiReturn_SUCCESS)
				rawMaterial->SetDiffuseColor(glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b));
			// Specular Color
			aiColor3D specularColor;
			if (aiMat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == aiReturn_SUCCESS)
				rawMaterial->SetSpecularColor(glm::vec3(specularColor.r, specularColor.g, specularColor.b));
			// Emissive Color
			aiColor3D emissiveColor;
			if (aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor) == aiReturn_SUCCESS)
				rawMaterial->SetEmissiveColor(glm::vec3(emissiveColor.r, emissiveColor.g, emissiveColor.b));

			aiString path;
			// Albedo map
			if (aiMat->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == aiReturn_SUCCESS)
				rawMaterial->SetAlbedoTexturePath(directory + path.C_Str());
			// Diffuse map
			if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS)
				rawMaterial->SetDiffuseTexturePath(directory + path.C_Str());
			// Specular Map
			if (aiMat->GetTexture(aiTextureType_SPECULAR, 0, &path) == aiReturn_SUCCESS)
				rawMaterial->SetSpecularTexturePath(directory + path.C_Str());
			// Normal map
			if (aiMat->GetTexture(aiTextureType_NORMALS, 0, &path) == aiReturn_SUCCESS)
				rawMaterial->SetNormalTexturePath(directory + path.C_Str());
			// Height Map
			if (aiMat->GetTexture(aiTextureType_HEIGHT, 0, &path) == aiReturn_SUCCESS)
				rawMaterial->SetHeightTexturePath(directory + path.C_Str());
			// Emission Map
			if (aiMat->GetTexture(aiTextureType_EMISSIVE, 0, &path) == aiReturn_SUCCESS)
			{
				rawMaterial->SetEmissiveTexturePath(directory + path.C_Str());
				// NOTE: Temporary fix
				rawMaterial->SetEmissiveColor(glm::vec3(1));
			}

			return rawMaterial;
		}

		Interpolation AssimpToInterpolation(aiAnimInterpolation aiInterpolation)
		{
			Interpolation interpolation;
			switch (aiInterpolation)
			{
			case aiAnimInterpolation_Step:
				interpolation = Interpolation::Constant;
				break;
			case aiAnimInterpolation_Linear:
				interpolation = Interpolation::Linear;
				break;
			default:
				LOG_WARN("Unsupported Assimp interpolation type. Falling back to linear.");
				interpolation = Interpolation::Linear;
				break;
			}

			return interpolation;
		}

		Clip* GetAnimationClip(const aiAnimation* aiAnimation, const Skeleton* skeleton)
		{
			if (!skeleton)
				return nullptr;

			Clip* clip = new Clip();
			clip->SetName(aiAnimation->mName.C_Str());
			float ticksPerSecond  = static_cast<float>(aiAnimation->mTicksPerSecond);
			ticksPerSecond = (ticksPerSecond == 0.0f) ? 25.0f : ticksPerSecond;

			for (unsigned int channelIndex = 0; channelIndex < aiAnimation->mNumChannels; channelIndex++)
			{
				const aiNodeAnim* track = aiAnimation->mChannels[channelIndex];
				int jointId = skeleton->GetJointId(track->mNodeName.C_Str());

				if (jointId == -1)
					continue;

				TransformTrack& transformTrack = (*clip)[jointId];
				// Position track
				transformTrack.GetPositionTrack().SetSize(track->mNumPositionKeys);
				for (unsigned int i = 0; i < track->mNumPositionKeys; i++)
				{
					float timeInTicks = static_cast<float>(track->mPositionKeys[i].mTime);

					aiAnimInterpolation aiInterpolation = track->mPositionKeys[i].mInterpolation;
					transformTrack.GetPositionTrack().SetInterpolation(AssimpToInterpolation(aiInterpolation));

					aiVector3D aiVec3 = track->mPositionKeys[i].mValue;
					transformTrack.GetPositionTrack()[i].mTime = timeInTicks / ticksPerSecond;
					transformTrack.GetPositionTrack()[i].mValues[0] = aiVec3.x;
					transformTrack.GetPositionTrack()[i].mValues[1] = aiVec3.y;
					transformTrack.GetPositionTrack()[i].mValues[2] = aiVec3.z;
				}
				// Scale track
				transformTrack.GetScaleTrack().SetSize(track->mNumScalingKeys);
				for (unsigned int i = 0; i < track->mNumScalingKeys; i++)
				{
					float timeInTicks = static_cast<float>(track->mScalingKeys[i].mTime);

					aiAnimInterpolation aiInterpolation = track->mScalingKeys[i].mInterpolation;
					transformTrack.GetScaleTrack().SetInterpolation(AssimpToInterpolation(aiInterpolation));

					aiVector3D aiVec3 = track->mScalingKeys[i].mValue;
					transformTrack.GetScaleTrack()[i].mTime = timeInTicks / ticksPerSecond;
					transformTrack.GetScaleTrack()[i].mValues[0] = aiVec3.x;
					transformTrack.GetScaleTrack()[i].mValues[1] = aiVec3.y;
					transformTrack.GetScaleTrack()[i].mValues[2] = aiVec3.z;
				}
				// Rotation track
				transformTrack.GetRotationTrack().SetSize(track->mNumRotationKeys);
				for (unsigned int i = 0; i < track->mNumRotationKeys; i++)
				{
					float timeInTicks = static_cast<float>(track->mRotationKeys[i].mTime);

					aiAnimInterpolation aiInterpolation = track->mRotationKeys[i].mInterpolation;
					transformTrack.GetRotationTrack().SetInterpolation(AssimpToInterpolation(aiInterpolation));

					aiQuaternion aiQuat = track->mRotationKeys[i].mValue;
					transformTrack.GetRotationTrack()[i].mTime = timeInTicks / ticksPerSecond;
					transformTrack.GetRotationTrack()[i].mValues[0] = aiQuat.w;
					transformTrack.GetRotationTrack()[i].mValues[1] = aiQuat.x;
					transformTrack.GetRotationTrack()[i].mValues[2] = aiQuat.y;
					transformTrack.GetRotationTrack()[i].mValues[3] = aiQuat.z;
				}
			}

			return clip;
		}

		void ProcessNode(
			const aiNode* node,
			Pose& outPose,
			std::vector<int>& outParents,
			std::vector<std::string>& outJointNames,
			std::vector<glm::mat4>& outInvBindPoseMatrices,
			const std::unordered_map<std::string, glm::mat4>& boneNameToInvBindPoseMap, 
			int parentIndex)
		{
			std::string nodeName = node->mName.C_Str();

			auto it = boneNameToInvBindPoseMap.find(nodeName);
			if (it != boneNameToInvBindPoseMap.end())
			{
				aiVector3D scale;
				aiVector3D position;
				aiQuaternion rotation;
				node->mTransformation.Decompose(scale, rotation, position);

				Transform transform;
				transform.Scale = AssimpToGlm(scale);
				transform.Position = AssimpToGlm(position);
				transform.Rotation = AssimpToGlm(rotation);

				int currentIndex = outPose.AddJoint(transform);
				outParents.push_back(parentIndex);
				outJointNames.push_back(it->first);
				outInvBindPoseMatrices.push_back(it->second);

				parentIndex = currentIndex;
			}

			for (unsigned int i = 0; i < node->mNumChildren; i++)
				ProcessNode(node->mChildren[i], outPose, outParents, outJointNames, outInvBindPoseMatrices, boneNameToInvBindPoseMap, parentIndex);
		}

		Skeleton* GetSkeleton(const aiScene* aiScene)
		{
			std::unordered_map<std::string, glm::mat4> boneNameToInvBindPoseMap;

			for (unsigned int i = 0; i < aiScene->mNumMeshes; i++)
			{
				for (unsigned int j = 0; j < aiScene->mMeshes[i]->mNumBones; j++)
				{
					aiBone* bone = aiScene->mMeshes[i]->mBones[j];
					std::string boneName = bone->mName.C_Str();
					glm::mat4 invBindPose = AssimpToGlm(bone->mOffsetMatrix);
					boneNameToInvBindPoseMap.emplace(boneName, invBindPose);
				}
			}

			if (boneNameToInvBindPoseMap.size() == 0)
				return nullptr;

			glm::mat4 globalRootNodeInvMat = glm::inverse(AssimpToGlm(aiScene->mRootNode->mTransformation));

			Pose bindPose;
			std::vector<int> parents;
			std::vector<std::string> jointNames;
			std::vector<glm::mat4> invBindPoseMatrices;

			ProcessNode(aiScene->mRootNode, bindPose, parents, jointNames, invBindPoseMatrices, boneNameToInvBindPoseMap, -1);

			Skeleton* skeleton = new Skeleton(bindPose, parents, jointNames, invBindPoseMatrices, globalRootNodeInvMat);

			return skeleton;
		}
	}
}