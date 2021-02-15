// <copyright file="MyMgrPage.cpp" company="CNC Software, Inc.">
// Copyright (c) 2017 CNC Software, Inc. All rights reserved.
// </copyright>
// <summary>Implements the MyMgrPage class</summary>
//
#include "stdafx.h"
#include "MyMgrPage.h"
#include "Toolbar.h"
#include "OpMgrPage.h"  // GetChookResourceHandle()
// <summary> Gets the chains in an operation. </summary>
///
/// <param name="opID"> Identifier for the operation. </param>
///
/// <returns> Null if it fails, else the chains referenced by the operation. </returns>
bool AddChainsToOperation(chain_manager_info ChainManagerInfo)
{
	bool succf = false;

	chain_manager(&ChainManagerInfo, 0, &succf);
	return succf;
}

// <summary>
/// 
/// </summary>
/// <param name="entity"></param>
/// <param name="pt1"></param>
/// <param name="pt2"></param>
/// <param name="color"></param>
/// <param name="level"></param>
/// <param name="attribs"></param>
/// <returns></returns>
bool CreateLine(
	ent& entity,
	const p_3d& pt1,
	const p_3d& pt2,
	MC_BYTE color,
	int level,
	attributes attribs)
{
	// Don't create "zero length" lines!
	if (pt_equal(pt1, pt2))
	{
		return false;
	}

	bool succf = false;
	DB_LIST_ENT_PTR pDB = nullptr;
	entity.id = L_ID;
	entity.u.li.e1 = pt1;
	entity.u.li.e2 = pt2;
	store_ent(&entity, &pDB, ALIVE_BIT, color, level, attribs, &succf);
	return succf;
}

void GetMinX(DB_LIST_ENT_PTR eptr, double minX)
{
	if (eptr == nullptr || eptr->eptr->id != SOLID_ID)
	{

	}
	minX = eptr->eptr->u.sld.min[0];
}
/// <summary>
/// Get the sld structs maxZ value from entity referenced in the class DB_LIST_ENT_PTR variable
/// </summary>
void GetMaxX(DB_LIST_ENT_PTR eptr, double& maxX)
{
	if (eptr == nullptr || eptr->eptr->id != SOLID_ID)
	{

	}
	maxX = eptr->eptr->u.sld.max[0];
}void GetMinY(DB_LIST_ENT_PTR eptr, double minY)
{
	if (eptr == nullptr || eptr->eptr->id != SOLID_ID)
	{

	}
	minY = eptr->eptr->u.sld.min[1];
}
/// <summary>
/// Get the sld structs maxZ value from entity referenced in the class DB_LIST_ENT_PTR variable
/// </summary>
void GetMaxY(DB_LIST_ENT_PTR eptr,double & maxY)
{
	if (eptr == nullptr || eptr->eptr->id != SOLID_ID)
	{
		
	}
	maxY = eptr->eptr->u.sld.max[1];
}
void GetMinZ(DB_LIST_ENT_PTR eptr, double minZ)
{
	if (eptr == nullptr || eptr->eptr->id != SOLID_ID)
	{

	}
	minZ = eptr->eptr->u.sld.min[2];
}
/// <summary>
/// Get the sld structs maxZ value from entity referenced in the class DB_LIST_ENT_PTR variable
/// </summary>
void GetMaxZ(DB_LIST_ENT_PTR eptr,double & maxZ)
{
	if (eptr == nullptr || eptr->eptr->id != SOLID_ID)
	{
		
	}
	maxZ = eptr->eptr->u.sld.max[2];
}
/// <summary>
/// Get the sld structs x , y & zLength(s) value from entity referenced in the class DB_LIST_ENT_PTR variable
/// </summary>
void GetXYZLength(DB_LIST_ENT_PTR eptr,double&xDistance,double &yDistance,double&zDistance)
{
	if (eptr == nullptr || eptr->eptr->id != SOLID_ID)
	{
	}
	auto minxP3D = P3D(eptr->eptr->u.sld.min[0], 0.0, 0.0);
	auto maxxP3D = P3D(eptr->eptr->u.sld.max[0], 0.0, 0.0);
	xDistance = minxP3D.Distance_P3D_P3D(maxxP3D);
	auto minyP3D = P3D(0.0, eptr->eptr->u.sld.min[1], 0.0);
	auto maxyP3D = P3D(0.0, eptr->eptr->u.sld.max[1], 0.0);
	yDistance = minyP3D.Distance_P3D_P3D(maxyP3D);
	auto minzP3D = P3D(0.0, 0.0, eptr->eptr->u.sld.min[2]);
	auto maxzP3D = P3D(0.0, 0.0, eptr->eptr->u.sld.max[2]);
	zDistance = minzP3D.Distance_P3D_P3D(maxzP3D);


}
#pragma region Spline Creation From Chains


#include "CrNB_CH.h" // make_nurbs_from_chains
#pragma comment (lib, "MCCurveSurfGeom.lib")
enum OriginalCurves
{
	Keep = 0,       //!<< Keep the original curves
	Blank = 1,      //!<< Blank the original curves
	Delete = 2,    //!<< Delete the original curves
	MoveToLevel = 3 //!<< Move the original curves to the specified level
};

/// <summary> Makes NURBS curves from the supplied chain(s). </summary>
///
/// <remarks> The is a wrapper around the 'make_nurbs_from_chains' function in the SDK. </remarks>
///
/// <param name="chains">                         The chains ('original curves'). </param>
/// <param name="maxError">                       The maximum error. </param>
/// <param name="originalCurvesDisposition">      The original curves disposition. </param>
/// <param name="originalCurvesDispositionLevel"> The original curves disposition level. </param>
/// <param name="color">                          The color for the new spline(s). </param>
/// <param name="level">                          The level for the new splines(s). </param>
///
/// <returns> true if it succeeds, false if it fails. </returns>
EptrArray MakeSplineFromChain(
	CHAIN* chains,
	double maxError,
	OriginalCurves originalCurvesDisposition,
	int originalCurvesDispositionLevel,
	MC_BYTE color,
	int level)
{
	EptrArray newEptrsArray;
	if (chains)
	{
		bool succf = false;
		/* CrNB_CH.h ->
		* @par Purpose: Make NURB Spline(s) from chains
		* @param[in]        chains      Chains to make NURB spline(s) from
		* @param[in]        max_error   Tolerance (only applies to higher than cubic curves)
		* @param[in]        disp_orig   Keep(0), Blank(1), Delete originals(2), or Move to a new level
		* @param[in]        disp_level  New level if disp_orig == 3
		* @param[in]        color       Color of new NURB spline(s)
		* @param[in]        level       Level of new NURB spline(s)
		* @param[out]       succf       TRUE indicates successful creation
		* @param[out]		newEptrArray	The array of new eptrs created
		* @param[in]		squash			(optional) true to squash to Cplane
		*/
		make_nurbs_from_chains(
			chains,
			maxError,
			static_cast <short> (originalCurvesDisposition),
			originalCurvesDispositionLevel,
			color,
			level,
			&succf,
			newEptrsArray);
	}
	//Sat ($0.40846 USD)
	return newEptrsArray;
}


#pragma endregion

#pragma region Create Flat Surface Imports

// Items imported directly from Mastercam for use in the SurfaceGeometry class

// err_codes - Replicated and defined in an enum in SurfaceGeometry.h
// #define T_ERR_PLANE	 1	 // = trouble finding plane
// #define T_ERR_DISJ	 2	 // = disjoint bndys
// #define T_ERR_SURF	 3	 // = trouble making surface
// #define T_ERR_NOPROJ  4	 // = no curves proj. on surf
// #define T_ERR_INTB	 5	 // = intersecting bndys
// #define T_ERR_MANY	 6	 // = too many pts, shorten step
// #define T_ERR_ALLOC	 7	 // = mem. alloc. problem
// #define T_ERR_ESC	 8	 // = BREAK entered
// #define T_ERR_NOINT	 9	 // = no intersection
// #define T_ERR_NO1	 10	 // = cant trim surface 1
// #define T_ERR_NO2	 11	 // = cant trim surface 2
// #define T_ERR_NEITHER 12  // = cant trim either surface
// #define T_ERR_OPEN	 13	 // = cant trim : open boundary found

//	trim_surf_from_flat_chains
//
//  Create a flat trimmed surface directly from closed 2D chain.
//
__declspec(dllimport) bool trim_surf_from_flat_chains(
	CHAIN* chains,
	DB_LIST_ENT_PTR* basesrf_ptr,
	surf_type* trim_surf,
	short* err_code);

#define KEEP_ORIG 0
#define BLNK_ORIG 1
#define DEL_ORIG  2

//  store_new_trim_surf
//
//  Puts a newly-made trimmed surface into the database,
//  disposes of the original surface,
//  and updates the special UNDO info.
//
__declspec(dllimport) bool store_new_trim_surf(
	surf_type* trim_surf,		// I: trim surface
	short dispose_orig,			// I: Keep (0), Blank(1), or Delete(2)
	bool use_constr_att,		// I: use current construction color, level, style, width?
	ent* orig_surf_ent,			// I: Original surface entity
	DB_LIST_ENT_PTR* trim_ptr); // O: Pointer to trimmed surface entity

// Be sure to include the required .LIBs
#pragma comment( lib, "MCCurveSurfGeom.lib") // (Mastercam 2021+)

class SurfaceGeometry
{
public:

	/// <summary> Values that represent flat boundary surface creation errors. </summary>
	enum class FlatBoundarySurfaceError
	{
		ERR_NONE = 0,
		ERR_PLANE = 1,	//!< trouble finding plane
		ERR_DISJ = 2,	//!< disjoint boundaries
		ERR_SURF = 3,	//!< trouble making surface
		ERR_NOPROJ = 4,	//!< no curves proj. on surf
		ERR_INTB = 5,	//!< intersecting boundaries
		ERR_MANY = 6,	//!< too many pts, shorten step
		ERR_ALLOC = 7,	//!< memory allocation problem
		ERR_ESC = 8,	//!< BREAK entered
		ERR_NOINT = 9,	//!< no intersection
		ERR_NO1 = 10,   //!< cant trim surface 1
		ERR_NO2 = 11,   //!< cant trim surface 2
		ERR_NEITHER = 12,   //!< cant trim either surface
		ERR_OPEN = 13,   //!< cant trim : open boundary found
	};

	/// <summary> Creates a Flat Boundary Surface. </summary>
	///
	/// <param name="pChain">	  Pointer to the (single) boundary Chain. </param>
	/// <param name="resultCode"> [out] The result (error) code. 0 = success, other = failure. </param>
	///
	/// <returns> Pointer to the (trimmed) Surface that was created. </returns>

};
#pragma endregion

#pragma region Create Flat Surface Function

/// <summary> Values that represent flat boundary surface creation errors. </summary>
enum class FlatBoundarySurfaceError
{
	ERR_NONE = 0,
	ERR_PLANE = 1,	//!< trouble finding plane
	ERR_DISJ = 2,	//!< disjoint boundaries
	ERR_SURF = 3,	//!< trouble making surface
	ERR_NOPROJ = 4,	//!< no curves proj. on surf
	ERR_INTB = 5,	//!< intersecting boundaries
	ERR_MANY = 6,	//!< too many pts, shorten step
	ERR_ALLOC = 7,	//!< memory allocation problem
	ERR_ESC = 8,	//!< BREAK entered
	ERR_NOINT = 9,	//!< no intersection
	ERR_NO1 = 10,   //!< cant trim surface 1
	ERR_NO2 = 11,   //!< cant trim surface 2
	ERR_NEITHER = 12,   //!< cant trim either surface
	ERR_OPEN = 13,   //!< cant trim : open boundary found
};
static DB_LIST_ENT_PTR CreateFlatBoundarySurface(CHAIN* pChain, FlatBoundarySurfaceError& resultCode)
{
	if (!pChain)
	{
		return nullptr;
	}
	DB_LIST_ENT_PTR pBaseSurf, pTrimSurf;
	NoStackEnt(baseSurfEent)
		surf_type trimSurf;
	short errCode = 0;
	resultCode = FlatBoundarySurfaceError::ERR_NONE;


	auto succf = trim_surf_from_flat_chains(pChain, &pBaseSurf, &trimSurf, &errCode);
	resultCode = static_cast<FlatBoundarySurfaceError> (errCode); // Set the [out] parameter.
	if (succf && errCode == 0)
	{
		// Put the new trimmed surface in the database.
		trimSurf.u.trim.psf = pBaseSurf;
		succf = (get_ent_from_eptr(pBaseSurf, &baseSurfEent) == 0);
		if (succf)
		{
			succf = store_new_trim_surf(&trimSurf, BLNK_ORIG, true, &baseSurfEent, &pTrimSurf);
		}
	}

	return ((errCode == 0) ? pTrimSurf : nullptr);
}

#pragma endregion

#pragma region Plane Manager


class PlaneMgr
{
public:
	/// <summary> The Return codes for the MakePlane Function </summary>
	/// <param name="NOERR | default == 0 | No errors occured "></param>
	/// <param name="NULL_ENT_PTS_ERR | default == 1 | POINT entities pointer is null"></param> 
	/// <param name=" NULL_PLANE_ERR | default == 2 | the array went out of bounds"></param> 
	enum class PlaneMgrErrorCodes
	{
		NOERR = 0,
		NULL_ENT_PTS_ERR = 1,
		NULL_PLANE_ERR = 2,
	};

	/// <summary>  Plane Constructor Params </summary> <- SUMMARY
	/// <param name="double zOffset | default  = 0.0 | the z value to set for the plane"></param> 
	/// <param name="long planeID | default == 0 | the long ID of the plane"></param> 
	/// <param name="bool Result1 | default == false | entity creation success flag "></param> 
	/// <param name="bool Result2 | default == false"></param>
	/// <param name="bool Result3 | default == false"></param>
	/// <param name="ent entity | default == {}"></param>
	/// <param name="ent entity2 | default == {}"></param>
	/// <param name="ent entity3 | default == {}"></param> 
	struct PlaneConstructorParams
	{
		double zOffset = 0.0;

		PlanePtr planePtr = nullptr;

		PlaneMgrErrorCodes errorCode = PlaneMgrErrorCodes::NOERR;

		bool Result1 = false;
		bool Result2 = false;
		bool Result3 = false;

		ent entity = {};
		ent entity2 = {};
		ent entity3 = {};

		//SldCurveSlicer::CurveSliceCallback callback = {};
	};
	/// <summary>
	/// Create a Mastercam Plane
	/// </summary>
	/// <param name="params"></param>
	static inline void MakePlane(PlaneMgr::PlaneConstructorParams& params)
	{
		//create the point entities to be used in the plane
		Create3Pts(params);
		//throw an exception if the points were not created
		if (!(params.Result1 && params.Result2 && params.Result3))
		{
			params.errorCode = PlaneMgrErrorCodes::NULL_ENT_PTS_ERR;
			return;
		}
		int succfFlag; // success flag
		p_2d tVal; // Tolerance value ?

		//instantiate a Mastercam plane class
		std::shared_ptr<Plane> pPlane = std::make_shared<Plane>(Cnc::Globals::Planes::GetPlanes(), params.entity, params.entity2, params.entity3, tVal, PlaneType::WCSPLANE, succfFlag);
		pPlane->SetTemporary(true);

		DeletePts(&params.entity, &params.entity2, &params.entity3);
		//if the plane object failed to instantiatethrow an error + cleanup
		if ((!succfFlag) || (!pPlane))
		{
			params.errorCode = PlaneMgrErrorCodes::NULL_PLANE_ERR;
			return;
		}
		ProcessPlane(params, pPlane);

	}
	/// <summary>Add the new supplied plane to the database</summary>
	/// <summary>Store the planes ID in the params struct</summary>
	/// <summary>Set the plane as the active WCS</summary>
	/// <summary>Execute the callback stored in the params struct</summary>
	/// <param name=" name == params | type == PlaneConstructorParams | descripion == the plane parameters "></param>
	/// <param name=" name == pPlane | type == PlanePtr | descripion == the plane pointer"></param>
	static inline void ProcessPlane(PlaneMgr::PlaneConstructorParams& params, PlanePtr& pPlane)
	{
		//Add the plane to the database
		Cnc::Globals::Planes::GetPlanes().Add(*pPlane);
		//Get the plane ID and store it in the params struct
		params.planePtr = pPlane;
		//Set the plane as the current Active Plane in Mastercam
		Cnc::Globals::Planes::GetPlanes().SetActivePlane(PlaneType::WCSPLANE, *pPlane);
	}
	/// <summary>
	/// Create a new MAstercam Point!
	/// </summary>
	/// <param name="point"></param>
	/// <param name="pt"></param> the 3d point struct , type name -> (p_3d)
	/// <param name="view"></param> the entity view
	/// <param name="color"></param> the entity color
	/// <param name="levelnumber"></param> the entities new level number
	/// <param name="attrib"></param> the entity attributes
	/// <returns>true if succesful</returns>
	static inline bool  MakePointEntity(ent* point, p_3d& pt, short view, MC_BYTE color, int levelnumber, attributes attrib)

	{
		bool succf = false;
		if (point)
		{
			DB_LIST_ENT_PTR ptr = nullptr;
			point->id = P_ID;
			point->u.pt = pt;
			store_ent(point, &ptr, 0, color, levelnumber, attrib, &succf);

		}
		return succf;
	};

	/// <summary>
	/// 
	/// </summary>
	/// <param name="incrAmount"></param>
	/// <param name="Result1"></param>
	/// <param name="Result2"></param>
	/// <param name="Result3"></param>
	/// <param name="entity1"></param>
	/// <param name="entity2"></param>
	/// <param name="entity3"></param>
	static inline void Create3Pts(PlaneConstructorParams& params)
	{
		auto topPlaneMatrix = Cnc::Globals::Planes::GetPlanes().GetTopPlanePtr()->GetMatrix33();

		auto topPlaneOrigin = Cnc::Globals::Planes::GetPlanes().GetTopPlanePtr()->GetOriginPoint();

		matrix_33 m33;

		topPlaneMatrix.Convert(m33);

		p_3d origin;

		origin.Set(topPlaneOrigin[0], topPlaneOrigin[1], topPlaneOrigin[2]);

		m33[0][2] = params.zOffset;

		m33[1][2] = params.zOffset;

		origin[2] = params.zOffset;

		params.Result1 = MakePointEntity(&params.entity, m33[0], 1, 1, 1, main_attrib);

		params.Result2 = MakePointEntity(&params.entity2, m33[1], 1, 1, 1, main_attrib);

		params.Result3 = MakePointEntity(&params.entity3, origin, 1, 1, 1, main_attrib);
	}
	/// <summary>
	/// Delete the poitn entities used to create the plane
	/// </summary>
	/// <param name="entity"></param>
	/// <param name="entity2"></param>
	/// <param name="entity3"></param>
	static inline void DeletePts(ent* entity, ent* entity2, ent* entity3)
	{
		if (entity && entity2 && entity3)
		{
			delete_ent(entity->eptr, false, false, false, false, true);
			delete_ent(entity2->eptr, false, false, false, false, true);
			delete_ent(entity3->eptr, false, false, false, false, true);
		}
	}
private:

};


#pragma endregion

#pragma region Silhouette Boundary Imports

/// <summary> The settings passed to Mastercam's CreateSilhouetteBoundaryGeometryMS. </summary>
/// <remarks> The active WCS controls where the CreateSilhouetteBoundaryGeometryMS will be used on the solid </remarks>
struct hmmSiluetBndyDat
{
	double filterTol;		// Filter tolerance
	double smoovAngle;		// Smooth angular tolerance used by Spline fit process
	double tessTol;			// Max chord height deviation for surface tessellation
	l_3d rotAxis;			// Rotation axis (default X-axis [0,0,0]-[1,0,0]
	int lookAhead;			// Number of pts to look ahead during filter process
	MC_BYTE profileType;	// 0=>upper spin profile; 1=> lower spin profile; 2=> full spin profile
	MC_BYTE spinBndy;		// 1 = 'spin silhouette', 2 = turn profile, 3 = Parasolid spin, 4 = Hybrid spin & silhouette boundary (for Interfacial only)
	bool sliceBndy;			// T=> use solid slice instead of spin for revolved profile
	bool trueArcFit;		// T=> use true arc fitting for spin boundary
	bool arcFitXY;			// T=> try to fit XY arcs during filter process
	bool arcFitXZ;			// T=> try to fit XZ arcs during filter process
	bool arcFitYZ;			// T=> try to fit YZ arcs during filter process
	bool splineFit;			// T=> try to fit splines to silhouette boundary
	bool constrMode3D;		// T=> 3D construction mode status
	bool useXform;			// T=> if spin silhouette, then use transformation logic
	bool rotAxis3D;			// T=> 'rotAxis' does NOT lie in XY plane
	p_3d viewOrigin;		// c_origin, or WCS.pts[0].pt
	short viewNum;			// CONSTR_VIEW_N or WCS.view_n
	bool shiftedOrigin;		// (Planes.ActiveOriginIsOn (PlaneType::CPLANE) != 0) or WCS.pts[0].pt != (0.0, 0.0, 0.0)
};
#if C_H_VERSION >= 2200 // Mastercam 2020 or later

DllImpExp bool CreateSilhouetteBoundaryGeometryMS(hmmSiluetBndyDat* bndyDat, EptrArray& eptrs);
#else
DllImpExp bool CreateSilhouetteBoundaryGeometryMS(hmmSiluetBndyDat* bndyDat, EPTRARRAY& eptrs);
#endif

#pragma endregion

#pragma region Silhouette Boundary End User Defined Objects
enum class SilhBndyErrorCodes
{
	NO_ERR = 0,
	NULL_ENTS_ERR = 1,
};
/// <summary>  </summary>
///
/// <remarks>  </remarks>
///
/// <param name=" ">                         The chains ('original curves'). </param>
///
/// <returns>  </returns>
struct silhBndySLiceParams
{
	EptrArray boundaryEptrs = {};
	SilhBndyErrorCodes errCode = SilhBndyErrorCodes::NO_ERR;
};
#pragma endregion

#pragma region Programmatic Slice 

/// Create wireframe geometry using the Silhoutte boundary slice function to profile the part!


void CreateSilhouetteBoundarySlice(silhBndySLiceParams& silhbndysliceParams)
{
	p_3d originPoint;

	originPoint.Set(0, 0, 0);

	p_3d startPoint;

	startPoint.Set(0, 0, 0);

	p_3d endPoint;
	endPoint.Set(1, 0, 0);

	l_3d rotationAxis{ startPoint, endPoint };

	hmmSiluetBndyDat boundarySettings = {};

	boundarySettings.viewOrigin = originPoint;
	boundarySettings.shiftedOrigin = true;
	boundarySettings.filterTol = .0025;
	boundarySettings.smoovAngle = 1.0;
	boundarySettings.tessTol = .001;
	boundarySettings.rotAxis = rotationAxis;
	boundarySettings.lookAhead = 100;
	boundarySettings.profileType = 2;//2 for full profile
	boundarySettings.sliceBndy = true;
	boundarySettings.constrMode3D = true;
	boundarySettings.spinBndy = 1;
	boundarySettings.arcFitXY = true;
	boundarySettings.arcFitXZ = false;
	boundarySettings.arcFitYZ = false;
	boundarySettings.splineFit = true;
	boundarySettings.rotAxis3D = false;
	boundarySettings.viewNum = CONSTR_VIEW_N;

	CreateSilhouetteBoundaryGeometryMS(&boundarySettings, silhbndysliceParams.boundaryEptrs);

	if (silhbndysliceParams.boundaryEptrs.IsEmpty())
	{
		silhbndysliceParams.errCode = SilhBndyErrorCodes::NULL_ENTS_ERR;
	}
}
#pragma endregion

#pragma region Programmatic Silhouette Boundary
/// <summary>
/// Create a silhouette boundary
/// </summary>
/// <param name="boundaryEptrs"></param>

static inline void MakeSilhouetteBndy(silhBndySLiceParams& silhbndysliceParams)
{
	p_3d startPoint;
	startPoint.Set(0, 0, 0);

	p_3d endPoint;
	endPoint.Set(1, 0, 0);

	l_3d rotationAxis{ startPoint, endPoint };

	hmmSiluetBndyDat boundarySettings = {};
	boundarySettings.filterTol = .0025;
	boundarySettings.smoovAngle = 1;
	boundarySettings.tessTol = .001;
	boundarySettings.rotAxis = rotationAxis;
	boundarySettings.lookAhead = 100;
	boundarySettings.profileType = 2;
	boundarySettings.arcFitXY = true;
	boundarySettings.arcFitXZ = false;
	boundarySettings.arcFitYZ = false;
	boundarySettings.splineFit = true;
	boundarySettings.rotAxis3D = false;
	boundarySettings.viewNum = CONSTR_VIEW_N;
	boundarySettings.shiftedOrigin = false;

	CreateSilhouetteBoundaryGeometryMS(&boundarySettings, silhbndysliceParams.boundaryEptrs);

	if (silhbndysliceParams.boundaryEptrs.IsEmpty())
	{
		silhbndysliceParams.errCode = SilhBndyErrorCodes::NULL_ENTS_ERR;
	}
}
#pragma endregion

#pragma region Chaining
//----------------------------------------------------------
/// <summary> Construct a Chain from the supplied list of entities. </summary>
/// <param name="entities"> The (in order!) line and/or arc entities. </param>
/// <returns> The new chain if successful, else null. </returns>
CHAIN* ChainAllSelected(bool onlyClosedChains)
{
	CHAIN* chains = nullptr;
	// Save system settings.
	bool savedPlaneFilter = chain_plane_filter,
		savedLevelFilter = chain_level_filter,
		savedColorFilter = chain_color_filter,
		savedIgnoreDepths = chain_ignore_depths;
	short savedChainDirection = chain_direction;
	chain_plane_filter = chain_level_filter = chain_color_filter = chain_ignore_depths = false;
	chain_direction = CHAIN_DIR_CW;

	// Mark all visibly 'selected' (Line/Arc/Spline) geometry as selected for chaining.
	set_clr_sel_bits(TEMP2_BIT, SELECT_BIT, 0, SELECT_BIT, A_ID | S_ID | NB_ID | L_ID, true);

	auto result = chain_all_selected(&chains, onlyClosedChains, true) == CHAIN_OK;

	// Mark all geometry as unselected for chaining.
	set_clr_sel_bits(0, TEMP2_BIT, 0, TEMP2_BIT, ALL_ENTITIES_MASK, true);

	// Restore settings.
	chain_plane_filter = savedPlaneFilter;
	chain_level_filter = savedLevelFilter;
	chain_color_filter = savedColorFilter;
	chain_ignore_depths = savedIgnoreDepths;
	chain_direction = savedChainDirection;

	return(result) ? chains : nullptr;
}

/// <summary>
/// 
/// </summary>
/// <param name="toolType"></param>
/// <param name="name"></param>
/// <param name="number"></param>
/// <param name="shoulderDiameter"></param>
/// <param name="metric"></param>
/// <returns></returns>
static Cnc::Tool::TlToolMillPtr CreateTool(MCToolType toolType, const CString& name, long number, double shoulderDiameter, bool metric)
{

#if C_H_VERSION >= 2200 // Mastercam 2020 or later
	auto factory = Cnc::Tool::CreateITlAssemblyItemFactory();
#else
	auto factory = GetTlAssemblyItemFactory();
#endif
	auto tlToolMillPtr = factory->CreateMillTool(toolType, metric);

	// Set some of the properties on our Tool.
	tlToolMillPtr->SetToolNumber(number);
	tlToolMillPtr->SetOverallDiameter(shoulderDiameter);
	tlToolMillPtr->SetShoulderDiameter(shoulderDiameter);

	if (!name.IsEmpty())
	{
		tlToolMillPtr->SetName(name);
	}

	return tlToolMillPtr;
}

static Cnc::Tool::TlHolderPtr CreateHolder(const CString& name, bool metric)
{
#if C_H_VERSION >= 2200 // Mastercam 2020 or later
	auto factory = Cnc::Tool::CreateITlAssemblyItemFactory();
#else
	auto factory = GetTlAssemblyItemFactory();
#endif
	auto tlHolderPtr = factory->CreateMillHolder(metric);

	// Set some of the properties on our Holder.
	if (!name.IsEmpty())
	{
		tlHolderPtr->SetName(name);
	}

	tlHolderPtr->AddSegment(3, 3, .5);

	return tlHolderPtr;
}
/// <summary>
/// 
/// </summary>
/// <param name="toolPtr"></param>
/// <param name="holderPtr"></param>
/// <param name="name"></param>
/// <param name="spAssembly"></param>
/// <returns></returns>
static bool MakeToolAssembly(Cnc::Tool::TlToolMillPtr toolPtr, Cnc::Tool::TlHolderPtr holderPtr, const CString& name, Cnc::Tool::TlAssemblyPtr& spAssembly)
{
	Cnc::Tool::TlAssemblyPtr tempPtr = std::make_shared <Cnc::Tool::TlAssembly>();

	tempPtr->SetName(name);
	tempPtr->SetMainTool(toolPtr);
	tempPtr->SetMainHolder(holderPtr);

	// *** Important ***
	// Since our Assembly is no longer a "default" Assembly, we must to this before using
	// it in Mastercam. (Mastercam looks at default and non-default Tool Assembles differently.)
	// Set this property for 'false' on your new Assembly before using it in Mastercam.
	// (Adding it to the Tool Manager or a .tooldb file.)
	if (!tempPtr->SetAsDefaultAssembly(false))
	{
		return false;
	}

	spAssembly = tempPtr; // Set out [out] parameter

	return true;
}
/// <summary>
/// 
/// </summary>
/// <param name="toolPtr"></param>
/// <param name="holderPtr"></param>
/// <param name="name"></param>
/// <returns></returns>
static Cnc::Tool::TlAssemblyPtr CreateToolAssembly(Cnc::Tool::TlToolMillPtr toolPtr, Cnc::Tool::TlHolderPtr holderPtr, const CString& name)
{
#if C_H_VERSION >= 2200 // Mastercam 2020 or later
	auto factory = Cnc::Tool::CreateITlAssemblyFactory();
#else
	auto factory = GetTlAssemblyFactory();
#endif

	MCToolType toolType = static_cast<MCToolType> (toolPtr->GetMCToolType());
	bool metric = toolPtr->GetIsMetric();
	auto tlAssemblyPtr = factory->Create(toolType, metric);

	// Set some of the properties on our Holder.
	if (!name.IsEmpty())
	{
		tlAssemblyPtr->SetName(name);
	}

	tlAssemblyPtr->SetMainTool(toolPtr);
	tlAssemblyPtr->SetMainHolder(holderPtr);

	// *** Important ***
	// Since our Assembly is no longer a "default" Assembly, we must to this before using
	// it in Mastercam. (Mastercam looks at default and non-default Tool Assembles differently.)
	// Set this property for 'false' on your new Assembly before using it in Mastercam.
	// (Adding it to the Tool Manager or a .tooldb file.)
	tlAssemblyPtr->SetAsDefaultAssembly(false);

	return tlAssemblyPtr;
}
/// <summary>
/// 
/// </summary>
/// <param name="toolType"></param>
/// <param name="toolName"></param>
/// <returns></returns>
static Cnc::Tool::TlAssemblyPtr CreateToolAssembly(MCToolType toolType, CString toolName)
{
	// Using the Factory to create a "default" tool assembly of the specified type.
	////var factory = new TlAssemblyFactory();
#if C_H_VERSION >= 2200 // Mastercam 2020 or later
	auto factory = Cnc::Tool::CreateITlAssemblyFactory();
#else
	auto factory = GetTlAssemblyFactory();
#endif
	auto tlAssemblyPtr = factory->Create(toolType, false);
	tlAssemblyPtr->SetName(_T("My Assembly"));

	// To set the properties of the Tool in the Assembly, we'll to pull out a copy,
	// alter it, and then assign it back into the Assembly.
	auto tool = tlAssemblyPtr->GetMillTool();

	// Set some of the properties on our Tool.
	tool->SetToolNumber(123);
	if (!toolName.IsEmpty())
	{
		tool->SetName(toolName);
	}

	tlAssemblyPtr->SetMainTool(tool);

	// To set the properties of the Holder in the Assembly we'll to pull out a copy,
	// alter it, and then assign it back into the Assembly.
	auto holder = tlAssemblyPtr->GetMainHolder();

	// Set some of the properties on our Holder.
	holder->SetName(_T("My Holder!"));
	holder->AddSegment(3, 3, .5);
	tlAssemblyPtr->SetMainHolder(holder);

	// *** Important ***
	// Since our Assembly is no longer a "default" Assembly, we must to this before using
	// it in Mastercam. (Mastercam looks at default and non-default Tool Assembles differently.)
	// Set this property for 'false' on your new Assembly before using it in Mastercam.
	// (Adding it to the Tool Manager or a .tooldb file.)
	tlAssemblyPtr->SetAsDefaultAssembly(false);

	return tlAssemblyPtr;
}
/// <summary>
/// 
/// </summary>
/// <param name="toolType"></param>
/// <param name="number"></param>
/// <param name="toolName"></param>
/// <param name="diameter"></param>
/// <param name="metric"></param>
/// <returns></returns>
static Cnc::Tool::TlAssemblyPtr BuildToolAssembly(MCToolType toolType, long number, CString toolName, double diameter, bool metric)
{
	auto tool = CreateTool(toolType, toolName, number, diameter, metric);
	auto holder = CreateHolder(_T("My Holder"), metric);
	Cnc::Tool::TlAssemblyPtr assembly;
	bool result = MakeToolAssembly(tool, holder, _T("My Assembly!"), assembly);
	return result ? assembly : nullptr;
}
/// <summary>
/// 
/// </summary>
/// <param name="toolAssemblyPtr"></param>
/// <returns></returns>
static bool AddToPartToolList(Cnc::Tool::TlAssemblyPtr toolAssemblyPtr)
{
	bool result = false;

	if (toolAssemblyPtr)
	{
		long machGroupID = 0;

		// If the Machine Group ID set in the tool data is '0',
		// we'll retrieve the ID of the "active" Machine Group and use that.
		// * This requirement of setting the Machine Group ID in the tool data
		//   for these 'Add' methods will be changing n the future. *
		// They will default to ADDing to the currently "active" Machine Group,
		// (like we're doing here) if the Machine Group ID in the tool assembly is '0'.
		if (toolAssemblyPtr->GetMachineGroup() == NULL_ID)
		{
			auto machineGroup = OMgetActiveMachineGroup(false);
			if (machineGroup)
			{
				machGroupID = machineGroup->grp_idn;
			}
		}
		else
		{
			// Make sure that the Machine Group ID in the incoming tool data
			// really matches an existing Machine Group.
			auto mainGroupList = TpMainGrpMgr.GetMainGrpList();
			auto group = mainGroupList.GroupByID(toolAssemblyPtr->GetMachineGroup());
			if (group)
			{
				if (mainGroupList.IsParentGroup(group->grp_idn))
				{
					machGroupID = group->grp_idn;
				}
			}
		}

		if (machGroupID > 0)
		{
			toolAssemblyPtr->ResetIDs();
			bool userCancelled = false;
			result = Cnc::Tool::GetToolSystem()->AddToMachineGroup(toolAssemblyPtr, machGroupID, userCancelled, false, true, true, true);
		}
	}

	return result;
}
/// <summary>
/// 
/// </summary>
/// <param name="assemblyPtr"></param>
/// <param name="op"></param>
/// <returns></returns>
static bool AssignToolToOperation(Cnc::Tool::TlAssemblyPtr& assemblyPtr, operation& op)
{
	/// <summary> Assigns the specified tool assembly to the provided operation. </summary>
	///
	/// <param name="spInput">			   The tool assembly to be assigned. </param>
	/// <param name="result">			   The operation to have tool assembly information assigned to it. </param>
	/// <param name="setOffsets">		   Flag specifying how to initialize the operation tool offsets. 0 - from the
	/// 								   tool, 1 - from the configuration. </param>
	/// <param name="setStation">		   true to initialize the operation tool station. </param>
	/// <param name="setSteps">			   true to set the step/peck/coolant values from the tool. </param>
	/// <param name="calcFeedSpeed">	   Flag specifying how to recalculate the feeds/speeds. 0 - From the workpiece
	/// 								   material, 1 - From the tool, 2 - From defaults. </param>
	/// <param name="alwaysLockFeedrates"> true to always lock feedrates. </param>
	///
	/// <returns> true if it succeeds, false if it fails. </returns>

	int setOffsets = 0;
	bool setStation = false;
	bool setSteps = false;
	int calcFeedSpeed = 1;
	bool alwaysLockFeedrates = false;

	return Cnc::Tool::GetToolSystem()->AssignToOperation(assemblyPtr, op, setOffsets, setStation,
		setSteps, calcFeedSpeed, alwaysLockFeedrates);
}
/// <summary> Adds a tool (assembly) to the operation. </summary>
///
/// <param name="op"> The operation. </param>
///
/// <returns> True if it succeeds, false if it fails. </returns>
bool AddTool(operation* op, int number, double diameter)
{
	bool succf = false;
	if (op)
	{
		auto tlAssembly = BuildToolAssembly(MCToolType::FlatEndmill, number, _T("Flat Endmill"), diameter, false);
		if (tlAssembly)
		{
			if (AddToPartToolList(tlAssembly))
			{
				succf = AssignToolToOperation(tlAssembly, *op);
			}
		}
	}

	return succf;
}

/// <summary>
/// Imported directly from Matercam OMrefreshOpManTree
/// </summary>
/// <param name="force"></param>
/// <returns></returns>
DllImpExp int OMrefreshOpManTree(bool force);
/// <summary>
/// Imported directly from Matercam OMenableRefresh
/// </summary>
/// <param name="on"></param>
/// <returns></returns>
DllImpExp bool OMenableRefresh(bool on);
/// <summary>
/// Imported directly from Matercam OMrefreshOperation
/// </summary>
/// <param name="op_idn"></param>
/// <param name="add_new"></param>
/// <returns></returns>
DllImpExp op_group* OMcreateGroup(LPCTSTR gname, MC_BYTE product,
	long parent_grp_idn = 0, bool edit_properties = false, bool use_insert_arrow = true);
/// <summary>
/// 
/// </summary>
/// <returns></returns>
extern "C" op_group * GetActiveMachineGroup()
{
	return  OMgetActiveMachineGroup(false);
}
/// <summary>
/// 
/// </summary>
/// <param name="product"></param>
/// <returns></returns>
extern "C" op_group * CreateMachineGroup(MC_BYTE product)
{
	return  OMcreateGroup(nullptr, product, 0, false, false);
}
bool VerifyMachineGroup(bool createIfNeeded)
{
	auto machGroup = GetActiveMachineGroup();
	if (machGroup && (machGroup->product == PRODUCT_MILL || machGroup->product == PRODUCT_ROUTER))
	{
		return true;
	}

	if (!createIfNeeded)
	{
		return true;
	}

	if (!CreateMachineGroup(PRODUCT_MILL))
	{
		MessageBox(get_MainFrame()->GetSafeHwnd(), _T("No Machine group created! Aborting process!"), _T("Error"), MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
/// <summary>
/// 
/// </summary>
/// <param name="op"></param>
void SetDefaultOpPktParams(operation& op, double maxZ,double feedRate,double plungeRate,double spindleSpeed,double retractRate)
{
	op.tl.feed = feedRate;

	op.tl.rpm = spindleSpeed;

	op.tl.plunge = plungeRate;

	op.tl.retract = retractRate;

	op.cmn.depth_inc = true;

	op.cmn.depth = 0.0;

	op.cmn.stk_remain = 0.0;

	op.dcuts.tool_down = true;

	op.lead.on = false;

	op.dcuts.stock_t_l = 0.0;

	op.misc.on = false;
	op.db.select_flag = true;
	op.cmn.retract_inc = false;
	op.cmn.clearance_inc = false;
	op.cmn.top_stock_inc = false;
	op.cmn.top_stock = maxZ;
	op.cmn.clearance_pln = maxZ + 1.00;
	op.cmn.retract_pln = maxZ + 1.00;
	op.cmn.feed_pln = maxZ + .25;
	op.cmn.feed_inc = false;
	op.cmn.depth_inc = true;
	op.cmn.depth = 0.0;
	op.db.select_flag = true;

	op.cmn.retract_inc = false;

	op.cmn.clearance_inc = false;

	op.cmn.top_stock_inc = false;

	op.cmn.top_stock = maxZ;

	op.cmn.clearance_pln = maxZ + 1.00;

	op.cmn.retract_pln = maxZ + 1.00;

	op.cmn.depth_inc = true;

	op.cmn.depth = 0.0;

	op.cmn.stk_remain = 0.0;

	op.u.prm_pkt.add_finish = false;

	op.u.prm_pkt.climb = false;

	op.u.prm_pkt.cp.lin_tol = .0001;

	op.u.prm_pkt.taper.on = false;

	op.u.prm_pkt.rough = true;

	op.u.prm_pkt.ch_sort.sort_method = 1;

	op.u.prm_pkt.co_spir_disp = true;

	op.u.prm_pkt.add_finish = true;

	op.u.prm_pkt.rgh_step_pct = 90;

	op.u.prm_pkt.rgh_entry.helix.angle = TWO_PI / 8;

	op.u.prm_pkt.rgh_entry.helix.angle = TWO_PI / 64;

	op.u.prm_pkt.rgh_entry.on = false;

	double min = .125, max = .5;

	short stk_mde = 2;

	short cut_mthd = 0;

	op.u.prm_pkt.cp.cmp_to_tip;

	double dbl = .0001;

	op.u.prm_pkt.cp.lin_tol = dbl;

	op.u.prm_pkt.cut_method = cut_mthd;

	op.u.prm_pkt.rough = true;

	op.u.prm_pkt.add_finish = false;

	op.u.prm_pkt.tool_down = true;

	short pck_typ = 0;

	op.dcuts.tool_down = true;

	op.u.prm_pkt.pock_type = pck_typ;

	op.u.prm_pkt.finish = true;

	op.u.prm_pkt.fin_n = 1;

	op.u.prm_pkt.fin_w_rgh = true;

	op.u.prm_pkt.fin_step = 0.05;

	op.u.prm_pkt.spiral_out = true;

	op.u.prm_pkt.rgh_step_pct_flat = true;

	op.u.prm_pkt.climb = false;

	op.u.prm_pkt.finish = true;

	op.u.prm_pkt.fin_n = 1;

	op.u.prm_pkt.fin_step = .01;

	op.lead.on = false;

	op.u.prm_pkt.rgh_entry.entry_type = 0;

	op.u.prm_pkt.rgh_entry.on = false;

	op.u.prm_pkt.fin_outer = true;

	op.u.prm_pkt.fin_w_rgh = false;

	op.u.prm_pkt.n_spring_cuts = 0;

	op.dcuts.stock_t_l = 0.0;

	op.u.prm_pkt.pock_type = 4;

	op.misc.on = false;
}
/// <summary>
/// 
/// </summary>
/// <param name="op"></param>
void SetDefaultOpContourParams(operation& op,double maxZ)
{
	op.db.select_flag = true;

	op.cmn.retract_inc = false;

	op.cmn.clearance_inc = false;

	op.cmn.top_stock_inc = false;

	op.cmn.top_stock = maxZ;

	op.cmn.clearance_pln = maxZ + 1.00;

	op.cmn.retract_pln = maxZ + 1.00;

	op.cmn.depth_inc = true;

	op.cmn.depth = 0.0;

	op.db.select_flag = true;

	op.cmn.retract_inc = false;

	op.cmn.clearance_inc = false;

	op.cmn.top_stock_inc = false;

	op.cmn.top_stock = maxZ;

	op.cmn.feed_inc = false;

	op.cmn.feed_pln = maxZ + .5;

	op.cmn.clearance_pln = maxZ + 1.00;

	op.cmn.retract_pln = maxZ + .25;

	op.cmn.depth_inc = true;

	op.cmn.depth = 0.0;

	op.cmn.stk_remain = 0.0;

	op.dcuts.tool_down = true;

	op.lead.on = false;

	op.dcuts.stock_t_l = 0.0;

	op.misc.on = false;

	op.cc.type = CC_OFF;

}

void Slice(double sliceDepth, double sliceIncAmount)
{


	auto wcsPlane = Cnc::Globals::Planes::GetPlanes().GetActivePlane(PlaneType::WCSPLANE);
	auto cPlane = Cnc::Globals::Planes::GetPlanes().GetActivePlane(PlaneType::CPLANE);
	auto tPlane = Cnc::Globals::Planes::GetPlanes().GetActivePlane(PlaneType::TPLANE);


	//Clear all entities marked with TEMP3_BIT
	set_clr_sel_bits(0, TEMP3_BIT, 0, TEMP3_BIT, ALL_ENTITIES_MASK, true);
	//Clear all SOLIDS marked with SELECT_BIT and mark them as TEMP_3
	set_clr_sel_bits(TEMP3_BIT, SELECT_BIT, 0, SELECT_BIT, SOLID_ID, true);
	//Clear all entities marked with SELECT_BIT
	set_clr_sel_bits(0, SELECT_BIT, 0, SELECT_BIT, ALL_ENTITIES_MASK, true);

	auto dbStart = db_start;
	do
	{
		//look past the "Dead" bits
		if (sel_bit_is_off(dbStart->eptr, ALIVE_BIT)) { dbStart = dbStart->next; continue; }
		//look past the entities that don't match the sel bit mask search pattern
		if (sel_bit_is_off(dbStart->eptr, TEMP3_BIT)) { dbStart = dbStart->next; continue; }
		//look past the entities that don't match the entity mask search pattern
		if (dbStart->eptr->id != SOLID_ID) { dbStart = dbStart->next; continue; }


		//do something with the SOLID
		auto zPos = dbStart->eptr->u.sld.max[2];







		VerifyMachineGroup(true);
		auto toolAssembly = BuildToolAssembly(MCToolType::FlatEndmill, 1, _T("1/8 EndMill Flat"), 0.1250, false);

		AddToPartToolList(toolAssembly);
		/*

		bool osuccf; DB_LIST_ENT_PTR optr = nullptr; operation cntrop; cntrop.opcode = TP_CONTOUR;

		operation_manager(&cntrop, OPMGR_INIT_ADD, &optr, &osuccf);

		SetDefaultOpContourParams(cntrop, zPos);

		AssignToolToOperation(toolAssembly, cntrop);

		*/









		CString zVal; zVal.Format(_T("%lf"), zPos);

		struct eptrType
		{
			EptrArray eptrArray;
			double zVal;
		};
		std::vector<eptrType>eptrArrayVector;

		sel_bit_turn_off(dbStart->eptr, TEMP3_BIT);
		write_ent_sel(dbStart->eptr, dbStart->eptr->eptr);



		while (zPos >= dbStart->eptr->u.sld.min[2])
		{

			if (zPos <= dbStart->eptr->u.sld.max[2] - sliceDepth)
			{
				break;
			}

			eptrType tp;

			tp.zVal = zPos;

			constr_depth = zPos;

			PlaneMgr::PlaneConstructorParams planeParams;

			planeParams.zOffset = zPos;

			constr_depth = zPos;

			zPos = zPos - sliceIncAmount;

			PlaneMgr::MakePlane(planeParams);

			if (planeParams.errorCode != PlaneMgr::PlaneMgrErrorCodes::NOERR) { break; }

			sel_bit_turn_on(dbStart->eptr, SELECT_BIT);
			write_ent_sel(dbStart->eptr, dbStart->eptr->eptr);

			silhBndySLiceParams sliceBndyParams;
			CreateSilhouetteBoundarySlice(sliceBndyParams);

			if (!sliceBndyParams.boundaryEptrs.IsEmpty())
			{
				tp.eptrArray = sliceBndyParams.boundaryEptrs;
				eptrArrayVector.push_back(tp);
			}

			sel_bit_turn_off(dbStart->eptr, SELECT_BIT);
			write_ent_sel(dbStart->eptr, dbStart->eptr->eptr);
			
		}
		EptrVector surfaces;

		CHAIN* newchains = nullptr;
		if (!eptrArrayVector.empty())
		{
			auto zPos = dbStart->eptr->u.sld.max[2];



			for (auto eptrArray : eptrArrayVector)
			{
				PlaneMgr::PlaneConstructorParams planeParams;

				planeParams.zOffset = eptrArray.zVal;

				constr_depth = eptrArray.zVal;

				PlaneMgr::MakePlane(planeParams);

				CHAIN* chains = nullptr;

				if (planeParams.errorCode != PlaneMgr::PlaneMgrErrorCodes::NOERR) { break; }

				for (auto s = 1; s < 50; s++)
				{
					for (auto i = 0; i < eptrArray.eptrArray.GetCount(); i++)
					{
						sel_bit_turn_on(eptrArray.eptrArray[i]->eptr, SELECT_BIT);
						write_ent_sel(eptrArray.eptrArray[i]->eptr, eptrArray.eptrArray[i]->eptr->eptr);
					}
					chains = nullptr;
					chains = ChainAllSelected(true);
					if (chains != nullptr)
					{
						for (auto i = 0; i < eptrArray.eptrArray.GetCount(); i++)
						{
							sel_bit_turn_on(eptrArray.eptrArray[i]->eptr, BLANK_BIT);
							write_ent_sel(eptrArray.eptrArray[i]->eptr, eptrArray.eptrArray[i]->eptr->eptr);
						}

						break;
					}
					chain_tol = (.001 * s);
				}


				CHAIN* chain = nullptr;
				if (chains)
				{
					do
					{
						copy_chain(chains, &chain);
						if (chain)
						{
							FlatBoundarySurfaceError error;
							auto surface = CreateFlatBoundarySurface(chain, error);
							if (surface)
							{
								surfaces.push_back(surface);

							}
						}
						free_chain(&chain);
						chains = chains->next;
					} while (chains != nullptr);
					for (auto surf : surfaces)
					{
						sel_bit_turn_on(surf->eptr, SELECT_BIT);
						write_ent_sel(surf->eptr, surf->eptr->eptr);
					}
					silhBndySLiceParams sliceBndyParams2;
					MakeSilhouetteBndy(sliceBndyParams2);

					for (auto surf : surfaces)
					{
						sel_bit_turn_off(surf->eptr, SELECT_BIT);
						write_ent_sel(surf->eptr, surf->eptr->eptr);
					}
				
				
				for (auto s = 1; s < 50; s++)
				{
					chain_tol = (.001 * s);
					for (auto i = 0; i < sliceBndyParams2.boundaryEptrs.GetCount(); i++)
					{
						sel_bit_turn_on(sliceBndyParams2.boundaryEptrs[i]->eptr, SELECT_BIT);
						write_ent_sel(sliceBndyParams2.boundaryEptrs[i]->eptr, sliceBndyParams2.boundaryEptrs[i]->eptr->eptr);
					}
					newchains = nullptr;


					newchains = ChainAllSelected(true);
					if (newchains != nullptr)
					{
						for (auto i = 0; i < sliceBndyParams2.boundaryEptrs.GetCount(); i++)
						{
							sel_bit_turn_on(sliceBndyParams2.boundaryEptrs[i]->eptr, BLANK_BIT);
							write_ent_sel(sliceBndyParams2.boundaryEptrs[i]->eptr, sliceBndyParams2.boundaryEptrs[i]->eptr->eptr);
						}

						break;
					}
				}
				}

				auto eptrSplArr = MakeSplineFromChain(newchains, .005, OriginalCurves::Delete, 1, main_color, main_level);
				free_chains(&chains);
				free_chains(&newchains);

























				//EptrArray eptrSplArr;

				NoStackEnt(Line)
					p_3d pt1; p_3d pt2;
				pt1.Set(dbStart->eptr->u.sld.min[0] - .0625, dbStart->eptr->u.sld.min[1] - .0625, constr_depth);
				pt2.Set(dbStart->eptr->u.sld.max[0] + .0625, dbStart->eptr->u.sld.min[1] - .0625, constr_depth);
				CreateLine(Line, pt1, pt2, MC_RED, main_level, main_attrib);
				eptrSplArr.Add(Line.eptr);
				pt1.Set(dbStart->eptr->u.sld.max[0] + .0625, dbStart->eptr->u.sld.min[1] - .0625, constr_depth);
				pt2.Set(dbStart->eptr->u.sld.max[0] + .0625, dbStart->eptr->u.sld.max[1] + .0625, constr_depth);
				CreateLine(Line, pt1, pt2, MC_RED, main_level, main_attrib);
				eptrSplArr.Add(Line.eptr);

				pt1.Set(dbStart->eptr->u.sld.min[0] - .0625, dbStart->eptr->u.sld.max[1] + .0625, constr_depth);
				pt2.Set(dbStart->eptr->u.sld.max[0] + .0625, dbStart->eptr->u.sld.max[1] + .0625, constr_depth);
				CreateLine(Line, pt1, pt2, MC_RED, main_level, main_attrib);
				eptrSplArr.Add(Line.eptr);

				pt1.Set(dbStart->eptr->u.sld.min[0] - .0625, dbStart->eptr->u.sld.min[1] - .0625, constr_depth);
				pt2.Set(dbStart->eptr->u.sld.min[0] - .0625, dbStart->eptr->u.sld.max[1] + .0625, constr_depth);
				CreateLine(Line, pt1, pt2, MC_RED, main_level, main_attrib);
				eptrSplArr.Add(Line.eptr);






				for (auto q = 0; q < eptrSplArr.GetCount(); q++)
				{

					sel_bit_turn_on(eptrSplArr[q]->eptr, SELECT_BIT);
					write_ent_sel(eptrSplArr[q]->eptr, eptrSplArr[q]->eptr->eptr);
				}







				auto finalchains = ChainAllSelected(true);

				if (number_of_chains(finalchains) >= 1)
				{


					Cnc::Globals::Planes::GetPlanes().SetActivePlane(PlaneType::WCSPLANE, wcsPlane);
					Cnc::Globals::Planes::GetPlanes().SetActivePlane(PlaneType::CPLANE, cPlane);
					Cnc::Globals::Planes::GetPlanes().SetActivePlane(PlaneType::TPLANE, tPlane);

					bool O_succf; DB_LIST_ENT_PTR O_ptr = nullptr; operation op; op.opcode = TP_POCKET;

					operation_manager(&op, OPMGR_INIT_ADD, &O_ptr, &O_succf);



					chain_manager_info ChainManagerInfo;

					ChainManagerInfo.chns = finalchains;

					ChainManagerInfo.op_idn = op.op_idn;

					ChainManagerInfo.mode = CHNMGR_ADD;

					AddChainsToOperation(ChainManagerInfo);

					AssignToolToOperation(toolAssembly, op);

					SetDefaultOpPktParams(op, constr_depth,50.0,7.5,550.0,50.0);

					operation_manager(&op, OPMGR_REWRITE, &O_ptr, &O_succf);

					operation_manager(&op, OPMGR_NCI_REGEN, &O_ptr, &O_succf);











				}
				else
				{

				}

					free_chains(&finalchains);
				for (auto q = 0; q < eptrSplArr.GetCount(); q++)
				{

					sel_bit_turn_on(eptrSplArr[q]->eptr, BLANK_BIT);
					write_ent_sel(eptrSplArr[q]->eptr, eptrSplArr[q]->eptr->eptr);
				}
				//for (auto i = 0; i < eptrArray.eptrArray.GetCount(); i++)
				//{
				//	sel_bit_turn_on(eptrArray.eptrArray[i]->eptr, BLANK_BIT);
				//	write_ent_sel(eptrArray.eptrArray[i]->eptr, eptrArray.eptrArray[i]->eptr->eptr);

				//}
			}
		}
		//get the next entity in the database // -> use get raw ent instead??
		dbStart = dbStart->next;
	} while (dbStart != nullptr);

	rebuild_graphics();
	Cnc::Globals::Planes::GetPlanes().SetActivePlane(PlaneType::WCSPLANE, wcsPlane);
	Cnc::Globals::Planes::GetPlanes().SetActivePlane(PlaneType::CPLANE, cPlane);
	Cnc::Globals::Planes::GetPlanes().SetActivePlane(PlaneType::TPLANE, tPlane);
}

#pragma endregion

#pragma endregion
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#ifdef UNICODE //Test to see if we're using wchar_ts or not.
typedef std::wstring StringType;
#else
typedef std::string StringType;
#endif
namespace FileManager
{
	StringType pathAppend(const StringType& p1, const StringType& p2);
	std::string wstrtostr(const std::wstring& wstr);
	double StringToDouble(StringType input);
	int StringToInt(StringType input);
	StringType GetExtension(StringType filename);
	StringType GetBaseFilename(StringType filename);
	StringType GetCurrentPath();
	std::vector<StringType> GetListOfFiles(StringType path);
	bool PathExists(const StringType& s);
	inline bool FileExists(const StringType& name);
	std::vector<StringType> split(StringType stringToBeSplitted, StringType delimeter);
}
//
///////////////////////////
///////////////////////////
//
//start of string functions//
//
///////////////////////////
///////////////////////////
//
StringType GlobalPath;
StringType FileManager::pathAppend(const StringType& p1, const StringType& p2) {

	char sep = '/';
	StringType tmp = p1;

#ifdef _WIN32
	sep = '\\';
#endif

	if (p1[p1.length()] != sep) { // Need to add a
		tmp += sep;                // path separator
		return(tmp + p2);
	}
	else
		return(p1 + p2);
}
std::string FileManager::wstrtostr(const std::wstring& wstr)
{
	std::string strTo;
	char* szTo = new char[wstr.length() + 1];
	szTo[wstr.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
	strTo = szTo;
	delete[] szTo;
	return strTo;
}
double FileManager::StringToDouble(StringType input)
{
	CString c_str = input.c_str();
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(c_str);
	// construct a std::string using the LPCSTR input
	std::string strStd(pszConvertedAnsiString);
	return atof(strStd.c_str()); /*c_str is needed to convert string to const char*/
}
int FileManager::StringToInt(StringType input)
{
	CString c_str = input.c_str();
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(c_str);
	// construct a std::string using the LPCSTR input
	std::string strStd(pszConvertedAnsiString);
	return atoi(strStd.c_str()); /*c_str is needed to convert string to const char*/
}

StringType FileManager::GetExtension(StringType filename)
{
	//store the position of last '.' in the file name
	int position = filename.find_last_of(_T("."));

	//store the characters after the '.' from the file_name string
	StringType result = filename.substr(position + 1);

	//print the result
	return result;
}
StringType FileManager::GetBaseFilename(StringType filename)
{
	size_t pos = filename.rfind(_T("."));
	if (pos == StringType::npos)  //No extension.
		return filename;

	if (pos == 0)    //. is at the front. Not an extension.
		return filename;

	return filename.substr(0, pos);
}
StringType FileManager::GetCurrentPath()
{
#ifdef UNICODE //Test to see if we're using wchar_ts or not.
	wchar_t buffer[MAX_PATH];
#else
	char buffer[MAX_PATH];
#endif
	GetModuleFileName(NULL, buffer, MAX_PATH);
	StringType::size_type pos = StringType(buffer).find_last_of(_T("\\/"));
	return StringType(buffer).substr(0, pos);
}
std::vector<StringType> FileManager::GetListOfFiles(StringType path)
{
	std::vector<StringType>stringvector;
	WIN32_FIND_DATA fileData;
	memset(&fileData, 0, sizeof(WIN32_FIND_DATA));
	CString string;
	string.Format(_T("%s\\*"), path.c_str());
	HANDLE handle = FindFirstFile(string, &fileData);

	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (_tcscmp(fileData.cFileName, _T(".")) != 0 && // ignore "." and ".."
				_tcscmp(fileData.cFileName, _T("..")) != 0)
			{
				stringvector.push_back(fileData.cFileName);
			}
		} while (FindNextFile(handle, &fileData));

		FindClose(handle);
	}

	return stringvector;
}
bool FileManager::PathExists(const StringType& s)
{
	struct stat buffer;

	CString c_str = s.c_str();
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(c_str);
	// construct a std::string using the LPCSTR input
	std::string strStd(pszConvertedAnsiString);
	return (stat(strStd.c_str(), &buffer) == 0);
}
inline bool FileManager::FileExists(const StringType& name)
{
	CString c_str = name.c_str();
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(c_str);
	// construct a std::string using the LPCSTR input
	std::string strStd(pszConvertedAnsiString);
	struct stat buffer;
	return (stat(strStd.c_str(), &buffer) == 0);
}


std::vector<StringType> FileManager::split(StringType stringToBeSplitted, StringType delimeter)
{
	std::vector<StringType> splittedString;
	int startIndex = 0;
	int  endIndex = 0;
	while ((endIndex = stringToBeSplitted.find(delimeter, startIndex)) < stringToBeSplitted.size())
	{

		StringType val = stringToBeSplitted.substr(startIndex, endIndex - startIndex);
		splittedString.push_back(val);
		startIndex = endIndex + delimeter.size();

	}
	if (startIndex < stringToBeSplitted.size())
	{
		StringType val = stringToBeSplitted.substr(startIndex);
		splittedString.push_back(val);
	}
	return splittedString;

}
//Get a list of the files in the specified folder
std::vector<StringType> GetListOfFolders(StringType path)
{

	std::vector<StringType>stringvector;

	WIN32_FIND_DATA fileData;

	memset(&fileData, 0, sizeof(WIN32_FIND_DATA));

	CString string;

	string.Format(_T("%s\\*"), path.c_str());

	HANDLE handle = FindFirstFile(string, &fileData);

	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (fileData.dwFileAttributes == 16) // Get access only on folders
			{
				if (_tcscmp(fileData.cFileName, _T(".")) != 0 && // ignore "." and ".."

					_tcscmp(fileData.cFileName, _T("..")) != 0)
				{
					stringvector.push_back(fileData.cFileName);
				}
			}
		} while (FindNextFile(handle, &fileData));
		FindClose(handle);
	}
	return stringvector;
}
//
///////////////////////////
///////////////////////////
//
//end of string functions//
//
///////////////////////////
///////////////////////////
//
//

//////////////////////////////////////////////////////////////////////////
// MyMgrPage
//////////////////////////////////////////////////////////////////////////
// MyMgrPage

IMPLEMENT_DYNAMIC (MyMgrPage, MCDialogBar)

BEGIN_MESSAGE_MAP (MyMgrPage, MCDialogBar)
	//{{AFX_MSG_MAP (MyMgrPage)
	
	// Dialog events
	ON_MESSAGE (WM_INITDIALOG, OnInitDialog)
	ON_WM_SIZE ()

	// Control events
	ON_BN_CLICKED (IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED (IDC_CHECK2, OnBnClickedCheck2)
	ON_UPDATE_COMMAND_UI_RANGE (ID_OPMGR_SELECT_ALL, ID_OPMGR_HELP, OnUpdateToolbarButtons) // optional
	// We need to "eat" a errant tooltip string from ID "101" for a BCG bug.
	ON_NOTIFY_EX_RANGE (TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE (TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &MyMgrPage::OnTvnSelchangedTree)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE, &MyMgrPage::OnTvnSelchangingTree)
END_MESSAGE_MAP ()


void MyMgrPage::DoDataExchange (CDataExchange *pDX)
	{
	MCDialogBar::DoDataExchange (pDX);

	//{{AFX_DATA_MAP (MyMgrPage)
	DDX_Control (pDX, IDC_CHECK1, m_CheckBox1);
	DDX_Control (pDX, IDC_CHECK2, m_CheckBox2);
	DDX_Check (pDX, IDC_CHECK1, m_Option1_State);
	DDX_Check (pDX, IDC_CHECK2, m_Option2_State);
	DDX_Control (pDX, IDC_EDIT1, m_EditCtrl1);
	DDX_Control (pDX, IDC_EDIT2, m_EditCtrl2);
	DDX_Text (pDX, IDC_EDIT1, m_Text1);
	DDX_Text (pDX, IDC_EDIT2, m_Text2);
	DDX_Control (pDX, IDC_TREE, m_Tree);
	DDX_Control (pDX, IDC_TOOLBAR_LOCATION, m_wndToolbarLocation);
	//}}AFX_DATA_MAP
	}

#pragma region Construction / Destruction

MyMgrPage::MyMgrPage () : MCDialogBar (MyMgrPage::IDD)
	, m_Option1_State (TRUE)
	, m_Option2_State (FALSE)
	, m_MyBarID (-1)
	{
	//{{AFX_DATA_INIT(MyMgrPage)
	//}}AFX_DATA_INIT
	}

MyMgrPage::~MyMgrPage ()
	{
	}

//////////////////////////////////////////////////////////////////////////

LRESULT MyMgrPage::OnInitDialog (WPARAM wParam, LPARAM lParam)
	{
	// If we've not already done this "upstream" from here...
	//ChangeResCl res (GetChookResourceHandle());
	
	MCDialogBar::OnInitDialog (wParam, lParam);

	EnableOptionsButton (true);

	SetupResizing (); // Setup the Dialog resizer

	// Enable the visual manager style
	EnableVisualManagerStyle ();
	
	// Create the toolbar
	MgrToolBar *pMgrToolBar = GetToolbar ();
	if (!pMgrToolBar->CreateMgrToolBar (this, IDR_TOOLBAR, IDR_TOOLBAR_24))
		{
		TRACE (_T("Failed to create manager pane toolbar!\n"));
		return -1;
		}

	// Set ToolBar position and size:
	CSize sizeToolBar = pMgrToolBar->CalcFixedLayout (FALSE, TRUE);
	CRect rectToolBar;
	m_wndToolbarLocation.GetWindowRect (&rectToolBar);
	ScreenToClient (&rectToolBar);
	pMgrToolBar->SetWindowPos (&wndTop, rectToolBar.left, rectToolBar.top,
		sizeToolBar.cx, sizeToolBar.cy, SWP_NOACTIVATE);

	UpdateData (FALSE);

	InitToolTip ();

	LoadTree ();
	TreeExpandCollapseAll (m_Tree, TVE_EXPAND);

	return TRUE; // return TRUE unless you set the focus to a control
	}

void MyMgrPage::InitToolTip (void)
	{
	ChangeResCl resource (GetChookResourceHandle ());

	// Create the ToolTip control.
	m_tooltip.Create (this, TTS_ALWAYSTIP | TTS_NOPREFIX);
	m_tooltip.Activate (TRUE);

	// Load up the ToolTip strings.
	CString tip;
	tip.LoadString (IDS_CHECKBOX_TIP_1);
	m_tooltip.AddTool (GetDlgItem (IDC_CHECK1), tip);
	tip.LoadString (IDS_CHECKBOX_TIP_2);
	m_tooltip.AddTool (GetDlgItem (IDC_CHECK2), tip);
	tip.LoadString (IDS_TEXTBOX_TIP_1);
	m_tooltip.AddTool (GetDlgItem (IDC_EDIT1), tip);
	tip.LoadString (IDS_TEXTBOX_TIP_2);
	m_tooltip.AddTool (GetDlgItem (IDC_EDIT2), tip);
	tip.LoadString (IDS_TREE_TIP);
	m_tooltip.AddTool (GetDlgItem (IDC_TREE), tip);
	}

MgrToolBar *MyMgrPage::GetToolbar ()
	{
	return &m_ToolBar;
	}

void MyMgrPage::OnUpdateToolbarButtons (CCmdUI *pCmdUI)
	{
	UIDialogbar::OnUpdateButtons (pCmdUI);

	// If we need to enable/disable a button based on some "data state", that can be done here.
	TRACE (_T ("\nMyMgrPage::OnUpdateToolbarButtons ID-> %i"), pCmdUI->m_nID);
	}

#pragma endregion

#pragma region Methods (public)

CString MyMgrPage::GetCaption ()
	{
	CString text;
	GetWindowText (text);
	return text;
	}

void MyMgrPage::SetCaption (LPCTSTR caption)
	{
	if (caption)
		{
		SetWindowText (caption);
		}
	}

BOOL MyMgrPage::OnToolTipText (UINT, NMHDR *pNMHDR, LRESULT *pResult)
	{
	ChangeResCl res (GetChookResourceHandle ());

	// BCG's BCGPDlgImpl has a hard-coded nInfoToolTipID that they set to 101.
	// Eating this message in the case of '101' fixes the display of this incorrect tip string.
	if (pNMHDR->idFrom == 101)
		{
		return TRUE;
		}

	return FALSE;
	}

void MyMgrPage::OnMenuSelection (const int selection)
	{
	switch (selection)
		{
		case ID_ATTRIBUTES_BACKGROUNDCOLOR:
			m_Tree.SetBackgroundColor ();
			break;

		case ID_ATTRIBUTES_LINECOLOR:
			m_Tree.SetLineColor ();
			break;

		case ID_ATTRIBUTES_FONT:
			m_Tree.SetFontAttributes ();
			break;

		case ID_ATTRIBUTES_RESTOREDEFAULTATTRIBUTES:
			m_Tree.SetDefaultAttributes ();
			break;
		}
	}

#pragma endregion

#pragma region Load/Save state

BOOL MyMgrPage::LoadState (LPCTSTR lpszProfileName, int nIndex, UINT uiID)
	{
	BOOL ret = FALSE;
	if (lpszProfileName != nullptr && uiID > 0)
		{
		MCDialogBar::LoadState (lpszProfileName, nIndex, uiID);
		}

	// TODO: Load the tree state

	return ret;
	}

BOOL MyMgrPage::SaveState (LPCTSTR lpszProfileName, int nIndex, UINT uiID)
	{
	BOOL ret = FALSE;
	if (lpszProfileName != nullptr && uiID > 0)
		{
		ret = MCDialogBar::SaveState (lpszProfileName, nIndex, uiID);
		}

	// TODO: Save the tree state

	return ret;
	}

#pragma endregion
#include "DialogHookMgr.h"
#pragma region Control events

void MyMgrPage::OnBnClickedCheck1 ()
	{
	DialogHookMgr Mgr;

	CString Text;

	CString Text2;


	m_EditCtrl1.GetWindowText(Text);

	m_EditCtrl2.GetWindowText(Text2);

	auto maxDepth = FileManager::StringToDouble(StringType(Text));

	auto sliceAmount = FileManager::StringToDouble(StringType(Text2));

	if (sliceAmount > 0.0 && maxDepth > 0.0)
	{
		Slice(maxDepth, sliceAmount);
	}
	
}

void MyMgrPage::OnBnClickedCheck2 ()
	{



	auto StorePoint = [&](ent& point, p_3d& pt, short view, MC_BYTE color, int levelnumber, attributes attrib)
	{
		bool succf;
		DB_LIST_ENT_PTR ptr = nullptr;
		point.id = P_ID;
		point.u.pt = pt;
		store_ent(&point, &ptr, 0, color, levelnumber, attrib, &succf);
		return succf;
	};
	auto cPlaneMatrix = Cnc::Globals::Planes::GetPlanes().GetActivePlanePtr(PlaneType::CPLANE)->GetMatrix33();
	auto cPlaneOrigin = Cnc::Globals::Planes::GetPlanes().GetActivePlanePtr(PlaneType::CPLANE)->GetOriginPoint();

	matrix_33 m33;
	cPlaneMatrix.Convert(m33);
	p_3d origin;
	origin.Set(cPlaneOrigin[0], cPlaneOrigin[1], cPlaneOrigin[2]);

	NoStackEnt(entity1)
		NoStackEnt(entity2)
		NoStackEnt(entity3)
		auto Result1 = StorePoint(entity1, m33[0], 1, 1, 1, main_attrib);
	auto Result2 = StorePoint(entity2, m33[1], 1, 1, 1, main_attrib);
	auto Result3 = StorePoint(entity3, origin, 1, 1, 1, main_attrib);

	if (Result1 && Result2 && Result3)
	{
		int succfFlag; // success flag
		p_2d tVal; // Tolerance value ?

		std::shared_ptr<Plane> pPlane = std::make_shared<Plane>(Cnc::Globals::Planes::GetPlanes(), entity1, entity2, entity3, tVal, PlaneType::CPLANE, succfFlag);
		if (succfFlag)
		{
			auto tPlane = *Cnc::Globals::Planes::GetPlanes().GetTopPlanePtr();
			Cnc::Globals::Planes::GetPlanes().Add(*pPlane);
			Cnc::Globals::Planes::GetPlanes().SetActivePlane(PlaneType::WCSPLANE, *pPlane);
			Cnc::Globals::Planes::GetPlanes().SetActivePlane(PlaneType::CPLANE, tPlane);
			Cnc::Globals::Planes::GetPlanes().SetActivePlane(PlaneType::TPLANE, tPlane);
		}
	}


	}

BOOL MyMgrPage::OnCommand (WPARAM wParam, LPARAM lParam)
	{
	if (HIWORD (wParam) == BN_CLICKED)
		{
		UINT uID = LOWORD (wParam);
		auto pToolBar = GetToolbar ();
		if (pToolBar)
			{
			if (pToolBar->OnButtonPress (uID))
				{
				return TRUE;
				}
			}

		// The command was not handled by the toolbar and...
		// If we have other controls we can handle them here and return TRUE if we did.
		}

	// Or just let the base calls OnCommand handle them.
	return MCDialogBar::OnCommand (wParam, lParam);
	}

BOOL MyMgrPage::PreTranslateMessage (MSG* pMsg)
	{
	// Let the ToolTip process this message.
	if (IsWindow (m_tooltip.m_hWnd))
		m_tooltip.RelayEvent (pMsg);

	switch (pMsg->message)
		{
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
			TRACE (_T ("\nPreTranslateMessage -> %i"), pMsg->message);
			// Do something special here?
			// return TRUE if we've handled the message.
			//return TRUE;
			break;
		}

	return MCDialogBar::PreTranslateMessage (pMsg);
	}

#pragma endregion

#pragma region Tree control

void MyMgrPage::ExpandCollapseTree (CTreeCtrl &tree, HTREEITEM hItem, UINT nCode)
	{
	if (hItem != nullptr && tree.ItemHasChildren (hItem))
		{
		tree.Expand (hItem, nCode);
		HTREEITEM hChild = tree.GetChildItem (hItem);
		while (hChild)
			{
			ExpandCollapseTree (tree, hChild, nCode);
			hChild = tree.GetNextItem (hChild, TVGN_NEXT);
			}
		}
	}

void MyMgrPage::TreeExpandCollapseAll (CTreeCtrl &tree, UINT nCode)
	{
	HTREEITEM hRootItem = tree.GetRootItem ();
	if (hRootItem != nullptr)
		{
		ExpandCollapseTree (tree, hRootItem, nCode);
		}
	}

void MyMgrPage::LoadTree()
{
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT;
	tvInsert.item.pszText = _T("Tree");
	HTREEITEM hRoot = m_Tree.InsertItem(&tvInsert);
		HTREEITEM hBranch = m_Tree.InsertItem(_T("Item"), 0, 0, hRoot, hRoot);
				m_Tree.InsertItem(_T("Nested Item"), hBranch);
		



#pragma endregion




}


#pragma endregion

#pragma region Dialog sizing

void MyMgrPage::OnSize (UINT nType, int cx, int cy)
	{
	MCDialogBar::OnSize (nType, cx, cy);
	m_resizer.Move (); // Do auto resizing

	// Adjust the layout of the dialog bar
	//AdjustLayout ();
	}

void MyMgrPage::AdjustLayout ()
	{
	// Call base class AdjustLayout ()
	MCDialogBar::AdjustLayout ();

	MgrToolBar *pMgrToolBar = GetToolbar ();
	
	if (pMgrToolBar && ::IsWindow (pMgrToolBar->GetSafeHwnd ()))
		{
		CRect barRect, clientRect, windowRect;

		// Get the client rect
		GetClientRect (&clientRect);

		pMgrToolBar->GetWindowRect (&barRect);
		ScreenToClient (&barRect);

		// Set toolbar position.
		pMgrToolBar->SetWindowPos (&wndTop, 0, 0, clientRect.Width (), barRect.Height (), SWP_NOACTIVATE);

		// Determine height of the toolbar.
		pMgrToolBar->GetWindowRect (&barRect);
		int toolbarHeight = 2 * barRect.Height () + 4;

		if (toolbarHeight != 0)
			{
			GetWindowRect (&windowRect);
			//AdjustTreeSize (windowRect.Width (), windowRect.Height (), toolbarHeight);
			}
		}
	}

void MyMgrPage::OnKillFocus (CWnd *pNewWnd)
	{
	TRACE (_T ("\nMyMgrPage -> *OnKillFocus* "));
	MCDialogBar::OnKillFocus (pNewWnd);
	}

void MyMgrPage::OnSetFocus (CWnd *pOldWnd)
	{
	TRACE (_T ("\nMyMgrPage -> *OnSetFocus*"));
	MCDialogBar::OnSetFocus (pOldWnd);
	}

void MyMgrPage::SetupResizing ()
	{
	// The following code is used to allow re-sizing of this dialog.
	// This block MUST appear before OnInitDialog () to persist control positioning.
	static CResizer::CBorderInfo sBorderInfo[] =
		{
			{
			IDC_STATIC1,
				{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},
				{CResizer::eFixed, IDC_MAIN, CResizer::eTop},
				{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},
				{CResizer::eFixed, IDC_MAIN, CResizer::eTop}
			},

			{
			IDC_CHECK1,
				{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},
				{CResizer::eFixed, IDC_MAIN, CResizer::eTop},
				{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},
				{CResizer::eFixed, IDC_MAIN, CResizer::eTop}
			},

			{
			IDC_CHECK2,
				{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},
				{CResizer::eFixed, IDC_MAIN, CResizer::eTop},
				{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},
				{CResizer::eFixed, IDC_MAIN, CResizer::eTop}
			},

			{
			IDC_EDIT1,
				{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},
				{CResizer::eFixed, IDC_MAIN, CResizer::eTop},
				{CResizer::eFixed, IDC_MAIN, CResizer::eRight},
				{CResizer::eFixed, IDC_MAIN, CResizer::eTop}
			},

			{
			IDC_EDIT2,
				{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},
				{CResizer::eFixed, IDC_MAIN, CResizer::eTop},
				{CResizer::eFixed, IDC_MAIN, CResizer::eRight},
				{CResizer::eFixed, IDC_MAIN, CResizer::eTop}
			},

			{
			IDC_TREE,
				{CResizer::eFixed, IDC_MAIN, CResizer::eLeft},
				{CResizer::eFixed, IDC_MAIN, CResizer::eTop},
				{CResizer::eFixed, IDC_MAIN, CResizer::eRight},
				{CResizer::eFixed, IDC_MAIN, CResizer::eBottom}
			}
		};

	const int nSize = sizeof (sBorderInfo) / sizeof (sBorderInfo[0]);
	m_bResizerInit = m_resizer.Init (m_hWnd, nullptr, sBorderInfo, nSize);
	}

#pragma endregion

void MyMgrPage::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM item;
	item = m_Tree.GetSelectedItem();

	if (!m_Tree.ItemHasChildren(item))
	{
		auto parentitem = m_Tree.GetParentItem(item);
		(StringType)m_Tree.GetItemText(item);
		
	
	}



	//Check the state of New Item
	if ((pNMTreeView->itemNew.state & TVIS_SELECTED))
	{
		// If the item is selected, so make sure OnSelchanged()
		// will "select" it !
		//m_Tree.SetItemState(item, UINT(~TVIS_SELECTED), UINT(TVIS_SELECTED));
	}
	else if (!(pNMTreeView->itemNew.state & TVIS_SELECTED))
	{
		// The New item is not selected, so make sure OnSelchanged()
		// will not "re-select" it !
	}

	*pResult = 0;
}


void MyMgrPage::OnTvnSelchangingTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	HTREEITEM item;
	item = m_Tree.GetSelectedItem();
	HTREEITEM itemNew = pNMTreeView->itemNew.hItem;
	HTREEITEM itemOld = pNMTreeView->itemOld.hItem;
	//Check the state of New Item
	if ((pNMTreeView->itemNew.state & TVIS_SELECTED))
	{
		// If the item is selected, so make sure OnSelchanged()
		// will "select" it !
		//m_Tree.SetItemState(item, UINT(~TVIS_SELECTED), UINT(TVIS_SELECTED));

	}
	else if (!(pNMTreeView->itemNew.state & TVIS_SELECTED))
	{
		// The New item is not selected, so make sure OnSelchanged()
		// will not "re-select" it !
	}
	*pResult = 0;
}
