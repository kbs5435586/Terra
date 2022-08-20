#pragma once

extern HINSTANCE	g_hInst;
extern	HWND		g_hWnd;
extern	HWND		g_MinihWnd;
extern unsigned int	g_iCameraMoveIdx;
extern bool			IsMouse;
extern bool			IsDeffered;
extern bool			IsListBox_StaticMesh ;
extern bool			IsListBox_DynamicMesh;
extern bool			IsCreative;
extern bool			IsRotation_EffectMesh;
extern bool			IsDrag;
extern bool			IsCameraMove;
extern float		fCameraSpeed;
extern float		fEffectMeshX;
extern float		fEffectMeshY;
extern float		fEffectMeshZ;
extern float		g_TimeDelta;
extern int			g_iEffectRotateNum;
extern int			TotlaPointIdx;
extern TOOL_MODE	eMode;
extern NAVI			eNavi;
extern NAVI_MODE	g_eNaviMode;