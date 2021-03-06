// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "ParticleSystem/ParticleFeature.h"
#include "ParticleSystem/ParticleEmitter.h"
#include "FeatureCollision.h"

namespace pfx2
{


//////////////////////////////////////////////////////////////////////////
class CFeatureChildBase : public CParticleFeature
{
public:
	virtual EFeatureType GetFeatureType() override
	{
		return EFT_Child;
	}

	void AddToComponent(CParticleComponent* pComponent, SComponentParams* pParams) override
	{
		pComponent->AddSubInstances.add(this);
		if (IsDelayed())
			pParams->m_maxTotalLIfe += pComponent->GetParentComponent()->GetComponentParams().m_maxParticleLife;
	}

	virtual bool IsDelayed() const { return false; }
};

//////////////////////////////////////////////////////////////////////////

class CFeatureChildOnBirth : public CFeatureChildBase
{
public:
	CRY_PFX2_DECLARE_FEATURE

	void AddSubInstances(const SUpdateContext& context) override
	{
		CParticleContainer& parentContainer = context.m_runtime.GetParentContainer();
		IFStream normAges = parentContainer.GetIFStream(EPDT_NormalAge);
		IFStream lifeTimes = parentContainer.GetIFStream(EPDT_LifeTime);

		THeapArray<SInstance> triggers(*context.m_pMemHeap);
		triggers.reserve(parentContainer.GetNumParticles());
		for (auto particleId : parentContainer.GetSpawnedRange())
		{
			const float delay = context.m_deltaTime - normAges.Load(particleId) * lifeTimes.Load(particleId);
			triggers.emplace_back(particleId, delay);
		}
		context.m_runtime.AddSubInstances(triggers);
	}
};

CRY_PFX2_IMPLEMENT_FEATURE_DEFAULT(CParticleFeature, CFeatureChildOnBirth, "Child", "OnBirth", colorChild, EFT_Child);

//////////////////////////////////////////////////////////////////////////

class CFeatureChildOnDeath : public CFeatureChildBase
{
public:
	CRY_PFX2_DECLARE_FEATURE

	bool IsDelayed() const override { return true; }

	void AddSubInstances(const SUpdateContext& context) override
	{
		CParticleContainer& parentContainer = context.m_runtime.GetParentContainer();
		
		IFStream normAges = parentContainer.GetIFStream(EPDT_NormalAge);
		IFStream lifeTimes = parentContainer.GetIFStream(EPDT_LifeTime);

		THeapArray<SInstance> triggers(*context.m_pMemHeap);
		triggers.reserve(parentContainer.GetNumParticles());
		for (auto particleId : parentContainer.GetFullRange())
		{
			const float normAge = normAges.Load(particleId);
			if (IsExpired(normAge))
			{
				const float overAge = (normAge - 1.0f) * lifeTimes.Load(particleId);
				const float delay = context.m_deltaTime - overAge;
				triggers.emplace_back(particleId, delay);
			}
		}
		context.m_runtime.AddSubInstances(triggers);
	}
};

CRY_PFX2_IMPLEMENT_FEATURE(CParticleFeature, CFeatureChildOnDeath, "Child", "OnDeath", colorChild);

//////////////////////////////////////////////////////////////////////////

class CFeatureChildOnCollide : public CFeatureChildBase
{
public:
	CRY_PFX2_DECLARE_FEATURE

	bool IsDelayed() const override { return true; }

	void AddSubInstances(const SUpdateContext& context) override
	{
		CParticleContainer& parentContainer = context.m_runtime.GetParentContainer();
		THeapArray<SInstance> triggers(*context.m_pMemHeap);
		triggers.reserve(parentContainer.GetNumParticles());
		
		const auto contactPoints = parentContainer.IStream(EPDT_ContactPoint);

		for (auto particleId : parentContainer.GetFullRange())
		{
			const SContactPoint contact = contactPoints.Load(particleId);
			if (contact.m_state.collided)
				triggers.emplace_back(particleId, contact.m_time);
		}
		context.m_runtime.AddSubInstances(triggers);
	}
};

CRY_PFX2_IMPLEMENT_FEATURE(CParticleFeature, CFeatureChildOnCollide, "Child", "OnCollide", colorChild);


}
