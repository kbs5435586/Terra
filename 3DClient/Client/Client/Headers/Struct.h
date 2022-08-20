#pragma once
typedef struct tagCamera_Desc
{
	D3DXVECTOR3		vEye; 
	D3DXVECTOR3		vAt;
	D3DXVECTOR3		vAxisY; 
}CAMERADESC;

typedef struct tagProjection_Desc
{
	float		fFovY; 
	float		fAspect;
	float		fNear;
	float		fFar;
}PROJDESC;

typedef struct tagVertex_ViewPort
{
	D3DXVECTOR4			vPosition;
	D3DXVECTOR2			vTexUV;
}VTXVIEWPORT;

typedef struct tagVertex_Texture
{
	D3DXVECTOR3			vPosition;
	D3DXVECTOR2			vTexUV;
}VTXTEX;

typedef struct tagVertex_Normal_Texture
{
	D3DXVECTOR3			vPosition;
	D3DXVECTOR3			vNormal;
	D3DXVECTOR2			vTexUV;
}VTXNORTEX;

typedef struct tagMesh_Texture
{
	LPDIRECT3DTEXTURE9			pDiffuseTexture;
	LPDIRECT3DTEXTURE9			pNormalTexture;
	LPDIRECT3DTEXTURE9			pSpecularTexture;
}MESHTEXTURE;

typedef struct tagSubset_Desc
{
	D3DXMATERIAL		Material;
	MESHTEXTURE			MeshTexture;
}SUBSETDESC;

typedef struct tagVertex_Color
{
	D3DXVECTOR3			vPosition;
	DWORD				dwColor;
}VTXCOL;

typedef struct tagVertex_Cube_Texture
{
	D3DXVECTOR3			vPosition;
	D3DXVECTOR3			vTexUV;
}VTXCUBETEX;

typedef struct tagPolygon16
{
	unsigned short		_0, _1, _2;
}POLYGON16;

typedef struct tagPolygon32
{
	unsigned long		_0, _1, _2;
}POLYGON32;

typedef struct tagD3DXMeshContainer_Derived : public D3DXMESHCONTAINER
{
	LPD3DXMESH			pOriginalMesh;
	// d3dxmaterial + texture
	SUBSETDESC*			pSubSetDesc;
	unsigned long		dwNumFrames;
	D3DXMATRIX**		ppCombinedTransformationMatrices;
	D3DXMATRIX*			pOffsetMatrices;
	D3DXMATRIX*			pRenderingMatrices; // pOffset * pCombined

}D3DXMESHCONTAINER_DERIVED;

typedef struct tagD3DXFrame_Derived : public D3DXFRAME
{
	D3DXMATRIX		CombinedTransformationMatrix;
}D3DXFRAME_DERIVED;
typedef struct tagMapInfo
{
	tagMapInfo() {};
	tagMapInfo(TCHAR* pChar, float fSizeX, float fSizeZ, float fInterval)
	{
		lstrcpy(m_strKey, pChar);
		m_fSizeX = fSizeX;
		m_fSizeZ = fSizeZ;
		m_fInterval = fInterval;
	}
	TCHAR	m_strKey[128];
	float	m_fSizeX;
	float	m_fSizeZ;
	float	m_fInterval;
}MAPINFO;

typedef struct tagColliderInfo
{
	BOX_TYPE				eBoxType;
	D3DXMATRIX				matLocalTransform;
	const D3DXMATRIX* pBoneMatrix;
	const D3DXMATRIX* pWorldMatrix;
	D3DXMATRIX*		pParentMatrix;

	tagColliderInfo() {	}
	// 1. 박스타입. 
	// 2. 로컬스페이스상에서의 추가적인 변환.(예:메시의 사이즈에 맞게 변환 or 내가 지정한 상태로 변환)
	// 3. 뼈의 행렬(콜라이더가 뼈에 붙어서 출력되야하는 경우.)
	// 4. 월드 행렬(콜라이더가 월드 스페이스상에서 어떻게 출력돼야하는 지)
	// 5. 부모 행렬(콜라이더가 어떤 부모를 사용하고 있는지)
	tagColliderInfo(BOX_TYPE InBoxType, D3DXMATRIX InLocalTransform
		, const D3DXMATRIX* pInBoneMatrix, const D3DXMATRIX* pInWorldMatrix, D3DXMATRIX* pInParentMatrix = nullptr)
		: eBoxType(InBoxType), matLocalTransform(InLocalTransform)
		, pBoneMatrix(pInBoneMatrix), pWorldMatrix(pInWorldMatrix)
		, pParentMatrix(pInParentMatrix)
	{ }
}COLLIDER_INFO;

typedef struct tagOBB
{
	D3DXVECTOR3			vPoint[8];
	D3DXVECTOR3			vCenter;
	D3DXVECTOR3			vAlignAxis[3]; // 박스에 평행한 축 세개.(투영받을 벡터)
	D3DXVECTOR3			vProjAxis[3]; // 박스의 중심에서 각 면을 바라보는 벡터 세개(투영한 길이를 더할 벡터)
}OBB;

typedef struct tagUnitInfo
{
	wchar_t*	pName;
	int			iAtt;
	int			iHp;
	int			iDef;
	int			iLevel;
}UNIT_INFO;


typedef struct tagPSystem
{
	D3DXVECTOR3			vPos;
	D3DXCOLOR			vColor;
	//D3DXVECTOR2			vTexUV;
}PSYSTEM;

typedef struct tagPsAttribute
{
	tagPsAttribute()
	{
		fLifeTime = 0.f;
		fAge = 0.f;
		isAlive = true;
	}
	D3DXVECTOR3		vPosition; //- 월드 스페이스 내의 파티클위치
	D3DXVECTOR3		vVelocity; // 파티클의 속도,보통 초당이동 단위로 기록한다.
	D3DXVECTOR3		vAcceleration;//파티클의 가속,보통 초당 이동단위로 기록한다
	D3DXVECTOR3		vDir;
	float			fLifeTime;     // how long the particle lives for before dying  
	float			fAge;          // current age of the particle  
	D3DXCOLOR		vColor;        // current color of the particle   
	D3DXCOLOR		vColorFade;    // how the color fades with respect to time
	bool			isAlive;
}PSATTRIBUTE;

struct BoundingBox
{
	BoundingBox()
	{
		_min.x = INFINITY;
		_min.y = INFINITY;
		_min.z = INFINITY;

		_max.x = -INFINITY;
		_max.y = -INFINITY;
		_max.z = -INFINITY;
	}

	bool isPointInside(D3DXVECTOR3& p)
	{
		if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
			p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
};


typedef struct tagTrailData
{
	D3DXVECTOR3	vPosition[2] = {};
	float		fTimeCount = 0.f;
	tagTrailData(const D3DXVECTOR3& matUpWorld, const D3DXVECTOR3& matDownWorld)
	{
		vPosition[0] = matUpWorld;
		vPosition[1] = matDownWorld;
	}
}TRAIL;