#pragma once

#include "ObjMan.h"

struct ICharacterInstance;

class CCharacterRenderNode : public ICharacterRenderNode, private Cry3DEngineBase
{
	friend class COctreeNode;

public:
	CCharacterRenderNode();
	virtual ~CCharacterRenderNode();

	//////////////////////////////////////////////////////////////////////////
	// Implement ICharacterRenderNode
	//////////////////////////////////////////////////////////////////////////
	virtual EERType     GetRenderNodeType() final { return eERType_Character; }
	//! Debug info about object.
	virtual const char* GetName() const final;
	virtual const char* GetEntityClassName() const final          { return "CharacterRenderNode"; };
	virtual string      GetDebugString(char type = 0) const final { return GetName(); }

	//! Sets render node transformation matrix.
	virtual void SetMatrix(const Matrix34& transform) final;

	//! Gets local bounds of the render node.
	virtual void       GetLocalBounds(AABB& bbox) final           { bbox = m_boundsLocal; }

	virtual Vec3       GetPos(bool bWorldOnly = true) const final { return m_matrix.GetTranslation(); };
	virtual const AABB GetBBox() const final                      { return m_boundsWorld; }
	virtual void       FillBBox(AABB& aabb) final                 { aabb = GetBBox(); }
	virtual void       SetBBox(const AABB& WSBBox) final          { m_boundsWorld = WSBBox; };
	virtual void       OffsetPosition(const Vec3& delta) final;
	virtual void       Render(const struct SRendParams& EntDrawParams, const SRenderingPassInfo& passInfo) final;

	//! Gives access to object components.
	virtual IMaterial*          GetEntitySlotMaterial(unsigned int nPartId, bool bReturnOnlyVisible = false, bool* pbDrawNear = NULL) final { return m_pMaterial; }
	virtual ICharacterInstance* GetEntityCharacter(unsigned int nSlot, Matrix34A* pMatrix = NULL, bool bReturnOnlyVisible = false) final    { return m_pCharacterInstance; }

	//! Get physical entity.
	virtual struct IPhysicalEntity* GetPhysics() const final                 { return m_pPhysicsEntity; };
	virtual void                    SetPhysics(IPhysicalEntity* pPhys) final { m_pPhysicsEntity = pPhys; };
	virtual void                    Physicalize(bool bInstant = false) final;

	//! Set override material for this instance.
	virtual void SetMaterial(IMaterial* pMat) final { m_pMaterial = pMat; };

	//! Queries override material of this instance.
	virtual IMaterial* GetMaterial(Vec3* pHitPos = NULL) const final { return nullptr; };
	virtual IMaterial* GetMaterialOverride() final                   { return m_pMaterial; };

	virtual float      GetMaxViewDist();

	virtual void  OnRenderNodeBecomeVisible(const SRenderingPassInfo& passInfo);
	virtual void  OnRenderNodeBecomeInvisible();

	virtual void SetCameraSpacePos( Vec3* pCameraSpacePos );

	virtual void       GetMemoryUsage(ICrySizer* pSizer) const {};
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Implement ICharacterRenderNode
	//////////////////////////////////////////////////////////////////////////
	virtual void SetCharacter(ICharacterInstance* pCharacter) final;
	virtual void SetCharacterRenderOffset(const QuatTS& renderOffset) final;
	//////////////////////////////////////////////////////////////////////////

private:
	void CalcNearestTransform(Matrix34 &transformMatrix, const SRenderingPassInfo& passInfo);

private:
	_smart_ptr<ICharacterInstance> m_pCharacterInstance;
	IPhysicalEntity*               m_pPhysicsEntity;

	// Override Material used to render node
	_smart_ptr<IMaterial> m_pMaterial;

	// World space tranformation
	Matrix34 m_matrix;
	// World space bounding box
	AABB     m_boundsWorld;
	// Local space bounding box
	AABB     m_boundsLocal;

	QuatTS   m_renderOffset;

	Vec3*    m_pCameraSpacePos = nullptr;
};
