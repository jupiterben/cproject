#include "export.h"
#include "asciitok.h"
#include <string.h>
#include <map>
#include "format.h"

int GetStaticFrame() { return 0; }

// Replace some characters we don't care for.
#define CTL_CHARS 31
#define SINGLE_QUOTE 39
TCHAR *FixupName(const TCHAR *name)
{
	return const_cast<TCHAR *>(name);
	/*static TCHAR buffer[256];
	TCHAR *cPtr;

	_tcscpy_s(buffer, name);
	cPtr = buffer;

	while (*cPtr)
	{
		if (*cPtr == _T('"'))
			*cPtr = SINGLE_QUOTE;
		else if (*cPtr <= CTL_CHARS)
			*cPtr = _T('_');
		cPtr++;
	}
	return buffer;*/
}

const TCHAR *GetMapID(Class_ID cid, int subNo)
{
	if (cid == Class_ID(0, 0))
	{
		return ID_ENVMAP;
	}
	else if (cid == Class_ID(DMTL_CLASS_ID, 0))
	{
		static std::map<int, const TCHAR *> subNoMap = {
			{ID_AM, ID_MAP_AMBIENT},
			{ID_DI, ID_MAP_DIFFUSE},
			{ID_SP, ID_MAP_SPECULAR},
			{ID_SH, ID_MAP_SHINE},
			{ID_SS, ID_MAP_SHINESTRENGTH},
			{ID_SI, ID_MAP_SELFILLUM},
			{ID_OP, ID_MAP_OPACITY},
			{ID_FI, ID_MAP_FILTERCOLOR},
			{ID_BU, ID_MAP_BUMP},
			{ID_RL, ID_MAP_REFLECT},
			{ID_RR, ID_MAP_REFRACT}};
		return subNoMap[subNo];
	}
	else
	{
		return ID_MAP_GENERIC;
	}
	return _T("");
}

json DumpUVGen(StdUVGen *uvGen)
{
	json dump(json::value_t::object);
	int mapType = uvGen->GetCoordMapping(0);

	static std::map<int, TCHAR *> MapTypeStr = {
		{UVMAP_EXPLICIT, ID_MAPTYPE_EXP},
		{UVMAP_SPHERE_ENV, ID_MAPTYPE_SPH},
		{UVMAP_CYL_ENV, ID_MAPTYPE_CYL},
		{UVMAP_SHRINK_ENV, ID_MAPTYPE_SHR},
		{UVMAP_SCREEN_ENV, ID_MAPTYPE_SCR}};
	dump[ID_MAPTYPE] = (MapTypeStr[mapType]);

	TimeValue t = GetStaticFrame();
	dump[ID_U_OFFSET] = uvGen->GetUOffs(t);
	dump[ID_V_OFFSET] = uvGen->GetVOffs(t);
	dump[ID_U_TILING] = uvGen->GetUScl(t);
	dump[ID_V_TILING] = uvGen->GetVScl(t);
	dump[ID_ANGLE] = uvGen->GetAng(t);
	dump[ID_BLUR] = uvGen->GetBlur(t);
	dump[ID_BLUR_OFFSET] = uvGen->GetBlurOffs(t);
	dump[ID_NOISE_AMT] = uvGen->GetNoiseAmt(t);
	dump[ID_NOISE_SIZE] = uvGen->GetNoiseSize(t);
	dump[ID_NOISE_LEVEL] = uvGen->GetNoiseLev(t);
	dump[ID_NOISE_PHASE] = uvGen->GetNoisePhs(t);
	return dump;
}

json DumpTexture(Texmap *tex, Class_ID cid, int subNo, float amt)
{
	json dump(json::value_t::object);

	TSTR className;
	tex->GetClassName(className);
	dump[_T("className")] = (className);
	dump[_T("mapID")] = (GetMapID(cid, subNo));

	TSTR texName = FixupName(tex->GetName());
	dump[ID_TEXNAME] = texName;
	TSTR classNameStr = FixupName(className);
	dump[ID_TEXCLASS] = classNameStr;
	// If we include the subtexture ID, a parser could be smart enough to get
	// the class name of the parent texture/material and make it mean something.
	dump[ID_TEXSUBNO] = subNo;
	dump[ID_TEXAMOUNT] = amt;
	// Is this a bitmap texture?
	// We know some extra bits 'n pieces about the bitmap texture
	if (tex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
	{
		auto bitmapTex = (BitmapTex *)tex;
		TSTR mapName = bitmapTex->GetMapName();
		TSTR mapNameStr = FixupName(mapName);
		dump[ID_BITMAP] = mapNameStr;

		StdUVGen *uvGen = bitmapTex->GetUVGen();
		if (uvGen)
		{
			dump[_T("uvGen")] = DumpUVGen(uvGen);
		}

		TextureOutput *texout = bitmapTex->GetTexout();
		if (texout)
		{
			dump[ID_TEX_INVERT] = texout->GetInvert();
		}

		std::map<int, TCHAR *> FilterMap = {
			{FILTER_PYR, ID_BMP_FILT_PYR},
			{FILTER_SAT, ID_BMP_FILT_SAT},
			{FILTER_NADA, ID_BMP_FILT_NONE}};
		dump[ID_BMP_FILTER] = (FilterMap[bitmapTex->GetFilterType()]);
	}

	std::vector<json> subTex;
	for (int i = 0; i < tex->NumSubTexmaps(); i++)
	{
		auto sub = DumpTexture(tex->GetSubTexmap(i), tex->ClassID(), i, 1.0f);
		subTex.push_back(sub);
	}
	dump[_T("subTexmaps")] = subTex;
	return dump;
}

json DumpPoint3Keys(Control *control);
json ExportGlobalInfo(Interface *ip)
{
	json dump(json::value_t::object);
	Interval range = ip->GetAnimRange();
	Interface14 *iface = GetCOREInterface14();

	auto firstFrame = range.Start() / GetTicksPerFrame();
	auto lastFrame = range.End() / GetTicksPerFrame();
	auto frameSpeed = GetFrameRate();
	auto ticksPerFrame = GetTicksPerFrame();
	dump[_T("firstFrame")] = firstFrame;
	dump[_T("lastFrame")] = lastFrame;
	dump[_T("frameSpeed")] = frameSpeed;
	dump[_T("ticksPerFrame")] = ticksPerFrame;

	auto envMap = ip->GetEnvironmentMap();
	if (envMap)
	{
		dump[_T("environmentMap")] = DumpTexture(envMap, Class_ID(0, 0), 0, 1.0f);
	}

	// Output background color
	Control *bgColCont = ip->GetBackGroundController();
	if (bgColCont && bgColCont->IsAnimated())
	{
		// background color is animated.
		dump[ID_ANIMBGCOLOR] = DumpPoint3Keys(bgColCont);
	}
	else
	{
		// Background color is not animated
		Color bgCol = ip->GetBackGround(GetStaticFrame(), FOREVER);
		dump[ID_STATICBGCOLOR] = Format(bgCol);
	}

	// Output ambient light
	Control *ambLightCont = ip->GetAmbientController();
	if (ambLightCont && ambLightCont->IsAnimated())
	{
		// Ambient light is animated.
		dump[ID_ANIMAMBIENT] = DumpPoint3Keys(ambLightCont);
	}
	else
	{
		// Ambient light is not animated
		Color ambLight = ip->GetAmbient(GetStaticFrame(), FOREVER);
		dump[ID_STATICAMBIENT] = Format(ambLight);
	}
	return dump;
}

// The Node Header consists of node type (geometry, helper, camera etc.)
// node name and parent node
void ExportNodeHeader(INode *node, TCHAR *type, json &dump)
{ // Node name
	TSTR nodeStr = FixupName(node->GetName());
	dump[ID_NODE_NAME] = nodeStr;

	//  If the node is linked, export parent node name
	INode *parent = node->GetParentNode();
	if (parent && !parent->IsRootNode())
	{
		TSTR parentName = FixupName(parent->GetName());
		dump[ID_NODE_PARENT] = parentName;
	}
}

void DumpMatrix3(const Matrix3 *m, json &dump)
{
	dump[_T("MatrixInRow")] = Format(*m);  
	// Decompose the matrix and dump the contents
	AffineParts ap;
	float rotAngle;
	Point3 rotAxis;
	float scaleAxAngle;
	Point3 scaleAxis;
	decomp_affine(*m, &ap);
	// Quaternions are dumped as angle axis.
	AngAxisFromQ(ap.q, &rotAngle, rotAxis);
	AngAxisFromQ(ap.u, &scaleAxAngle, scaleAxis);
	dump[ID_TM_POS] = Format(ap.t);
	dump[ID_TM_ROTAXIS] = Format(rotAxis);
	dump[ID_TM_ROTANGLE] = Format(rotAngle);
	dump[ID_TM_SCALE] = Format(ap.k);
	dump[ID_TM_SCALEAXIS] = Format(scaleAxis);
	dump[ID_TM_SCALEAXISANG] = Format(scaleAxAngle);
}
// transform
json ExportNodeTM(INode *node)
{
	Matrix3 pivot = node->GetNodeTM(GetStaticFrame());
	json dump(json::value_t::object);
	// Node name
	// Export TM inheritance flags
	DWORD iFlags = node->GetTMController()->GetInheritanceFlags();

	dump[ID_INHERIT_POS] = std::vector<int>({INHERIT_POS_X & iFlags ? 1 : 0,
											 INHERIT_POS_Y & iFlags ? 1 : 0,
											 INHERIT_POS_Z & iFlags ? 1 : 0});

	dump[ID_INHERIT_ROT] = std::vector<int>({INHERIT_ROT_X & iFlags ? 1 : 0,
											 INHERIT_ROT_Y & iFlags ? 1 : 0,
											 INHERIT_ROT_Z & iFlags ? 1 : 0});

	dump[ID_INHERIT_SCL] = std::vector<int>({INHERIT_SCL_X & iFlags ? 1 : 0,
											 INHERIT_SCL_Y & iFlags ? 1 : 0,
											 INHERIT_SCL_Z & iFlags ? 1 : 0});
	// Dump the full matrix
	if (!pivot.IsIdentity())
	{
		DumpMatrix3(&pivot, dump);
	}
	return dump;
}

BOOL IsKnownController(Control *cont)
{
	ulong partA, partB;

	if (!cont)
		return FALSE;

	partA = cont->ClassID().PartA();
	partB = cont->ClassID().PartB();
	if (partB != 0x00)
		return FALSE;

	switch (partA)
	{
	case TCBINTERP_POSITION_CLASS_ID:
	case TCBINTERP_ROTATION_CLASS_ID:
	case TCBINTERP_SCALE_CLASS_ID:
	case HYBRIDINTERP_POSITION_CLASS_ID:
	case HYBRIDINTERP_ROTATION_CLASS_ID:
	case HYBRIDINTERP_SCALE_CLASS_ID:
	case LININTERP_POSITION_CLASS_ID:
	case LININTERP_ROTATION_CLASS_ID:
	case LININTERP_SCALE_CLASS_ID:
		return TRUE;
	}
	return FALSE;
}

bool GetAlwaysSample() { return false; }
bool GetIncludeIKJoints() { return true; }

inline bool isControlTypeOf(Control *cont, ulong classID)
{
	return (cont->ClassID() == Class_ID(classID, 0)) ? true : false;
}

template <class KeyT>
json dumpControl(const TCHAR *contType, IKeyControl *ikc, const std::function<void(const KeyT &, json &)> &dumper)
{
	json dump(json::value_t::object);
	dump[ID_CONTROL_TYPE] = contType;

	std::vector<json> keyDumps;
	for (int i = 0; i < ikc->GetNumKeys(); i++)
	{
		KeyT key;
		ikc->GetKey(i, &key);
		json keyDump(json::value_t::object);
		keyDump[_T("time")] = key.time;
		keyDump[_T("value")] = Format(key.val);
		dumper(key, keyDump);
		keyDumps.push_back(keyDump);
	}
	dump["keys"] = keyDumps;
	return dump;
}

json DumpPosKeys(Control *cont)
{
	IKeyControl *ikc = GetKeyControlInterface(cont);
	if (!ikc)
		return json_null;

	if (isControlTypeOf(cont, TCBINTERP_POSITION_CLASS_ID))
	{
		return dumpControl<ITCBPoint3Key>(ID_CONTROL_POS_TCB, ikc, [](const ITCBPoint3Key &key, json &keyDump) {
			keyDump["tens"] = Format(key.tens);
			keyDump["cont"] = Format(key.cont);
			keyDump["bias"] = Format(key.bias);
			keyDump["easeIn"] = Format(key.easeIn);
			keyDump["easeOut"] = Format(key.easeOut);
		});
	}
	else if (isControlTypeOf(cont, HYBRIDINTERP_POSITION_CLASS_ID))
	{
		return dumpControl<IBezPoint3Key>(ID_CONTROL_POS_BEZIER, ikc, [](const IBezPoint3Key &key, json &keyDump) {
			keyDump["intan"] = Format(key.intan);
			keyDump["outtan"] = Format(key.outtan);
			keyDump["flags"] = Format(key.flags);
		});
	}
	else if (isControlTypeOf(cont, LININTERP_POSITION_CLASS_ID))
	{
		return dumpControl<ILinPoint3Key>(ID_CONTROL_POS_LINEAR, ikc, [](const ILinPoint3Key &key, json &keyDump) {
		});
	}
	return json_null;
}

json DumpRotKeys(Control *cont)
{
	IKeyControl *ikc = GetKeyControlInterface(cont);
	if (!ikc)
		return json_null;

	if (isControlTypeOf(cont, TCBINTERP_ROTATION_CLASS_ID))
	{
		return dumpControl<ITCBRotKey>(ID_CONTROL_ROT_TCB, ikc, [](const ITCBRotKey &key, json &keyDump) {
			keyDump["tens"] = Format(key.tens);
			keyDump["cont"] = Format(key.cont);
			keyDump["bias"] = Format(key.bias);
			keyDump["easeIn"] = Format(key.easeIn);
			keyDump["easeOut"] = Format(key.easeOut);
		});
	}
	else if (isControlTypeOf(cont, HYBRIDINTERP_ROTATION_CLASS_ID))
	{
		return dumpControl<IBezQuatKey>(ID_CONTROL_ROT_BEZIER, ikc, [](const IBezQuatKey &key, json &keyDump) {
		});
	}
	else if (isControlTypeOf(cont, LININTERP_ROTATION_CLASS_ID))
	{
		return dumpControl<ILinRotKey>(ID_CONTROL_ROT_LINEAR, ikc, [](const ILinRotKey &key, json &keyDump) {
		});
	}
	return json_null;
}

json DumpScaleKeys(Control *cont)
{
	IKeyControl *ikc = GetKeyControlInterface(cont);
	if (!ikc)
		return json_null;

	if (isControlTypeOf(cont, TCBINTERP_SCALE_CLASS_ID))
	{
		return dumpControl<ITCBScaleKey>(ID_CONTROL_SCALE_TCB, ikc, [](const ITCBScaleKey &key, json &keyDump) {
			keyDump["tens"] = Format(key.tens);
			keyDump["cont"] = Format(key.cont);
			keyDump["bias"] = Format(key.bias);
			keyDump["easeIn"] = Format(key.easeIn);
			keyDump["easeOut"] = Format(key.easeOut);
		});
	}
	else if (isControlTypeOf(cont, HYBRIDINTERP_SCALE_CLASS_ID))
	{
		return dumpControl<IBezScaleKey>(ID_CONTROL_SCALE_BEZIER, ikc, [](const IBezScaleKey &key, json &keyDump) {
			keyDump["intan"] = Format(key.intan);
			keyDump["outtan"] = Format(key.outtan);
			keyDump["flags"] = Format(key.flags);
		});
	}
	else if (isControlTypeOf(cont, LININTERP_SCALE_CLASS_ID))
	{
		return dumpControl<ILinScaleKey>(ID_CONTROL_SCALE_LINEAR, ikc, [](const ILinScaleKey &key, json &keyDump) {
		});
	}
	return json_null;
}

json DumpPoint3Keys(Control *cont)
{
	IKeyControl *ikc = GetKeyControlInterface(cont);
	if (!ikc)
		return json_null;

	if (isControlTypeOf(cont, TCBINTERP_POINT3_CLASS_ID))
	{
		return dumpControl<ITCBPoint3Key>(ID_CONTROL_POINT3_TCB, ikc, [](const ITCBPoint3Key &key, json &keyDump) {
			keyDump["tens"] = Format(key.tens);
			keyDump["cont"] = Format(key.cont);
			keyDump["bias"] = Format(key.bias);
			keyDump["easeIn"] = Format(key.easeIn);
			keyDump["easeOut"] = Format(key.easeOut);
		});
	}
	else if (isControlTypeOf(cont, HYBRIDINTERP_POINT3_CLASS_ID))
	{
		return dumpControl<IBezPoint3Key>(ID_CONTROL_POINT3_BEZIER, ikc, [](const IBezPoint3Key &key, json &keyDump) {
			keyDump["intan"] = Format(key.intan);
			keyDump["outtan"] = Format(key.outtan);
			keyDump["flags"] = Format(key.flags);
		});
	}
	else if (isControlTypeOf(cont, HYBRIDINTERP_COLOR_CLASS_ID))
	{
		return dumpControl<IBezPoint3Key>(ID_CONTROL_COLOR_BEZIER, ikc, [](const IBezPoint3Key &key, json &keyDump) {
			keyDump["intan"] = Format(key.intan);
			keyDump["outtan"] = Format(key.outtan);
			keyDump["flags"] = Format(key.flags);
		});
	}
	return json_null;
}

json ExportAnimKeys(INode *node)
{
	json dump(json::value_t::object);

	TSTR nodeName = FixupName(node->GetName());
	dump[ID_NODE_NAME] = nodeName;

	auto posControl = node->GetTMController()->GetPositionController();
	auto rotControl = node->GetTMController()->GetRotationController();
	auto scaleControl = node->GetTMController()->GetScaleController();

	if (IsKnownController(posControl) && posControl->NumKeys())
	{
		dump[_T("posControl")] = DumpPosKeys(posControl);
	}
	if (IsKnownController(rotControl) && rotControl->NumKeys())
	{
		dump[_T("rotControl")] = DumpRotKeys(rotControl);
	}
	if (IsKnownController(scaleControl) && scaleControl->NumKeys())
	{
		dump[_T("scaleControl")] = DumpScaleKeys(scaleControl);
	}
	return dump;
}

// Output float keys if this is a known float controller that
// supports key operations. Otherwise we will sample the controller
// once for each frame to get the value.
json DumpFloatKeys(Control *cont)
{
	IKeyControl *ikc = GetKeyControlInterface(cont);
	if (!ikc)
		return json_null;

	if (isControlTypeOf(cont, TCBINTERP_FLOAT_CLASS_ID))
	{
		return dumpControl<ITCBFloatKey>(ID_CONTROL_FLOAT_TCB, ikc, [](const ITCBFloatKey &key, json &keyDump) {
			keyDump["tens"] = Format(key.tens);
			keyDump["cont"] = Format(key.cont);
			keyDump["bias"] = Format(key.bias);
			keyDump["easeIn"] = Format(key.easeIn);
			keyDump["easeOut"] = Format(key.easeOut);
		});
	}
	else if (isControlTypeOf(cont, HYBRIDINTERP_FLOAT_CLASS_ID))
	{
		return dumpControl<IBezFloatKey>(ID_CONTROL_FLOAT_BEZIER, ikc, [](const IBezFloatKey &key, json &keyDump) {
			keyDump["intan"] = Format(key.intan);
			keyDump["outtan"] = Format(key.outtan);
			keyDump["flags"] = Format(key.flags);
		});
	}
	else if (isControlTypeOf(cont, LININTERP_FLOAT_CLASS_ID))
	{
		return dumpControl<ILinFloatKey>(ID_CONTROL_FLOAT_LINEAR, ikc, [](const ILinFloatKey &key, json &keyDump) {
		});
	}
	else
	{
		// Unknown controller, no key interface or sample on demand -
		// This might be a procedural controller or something else we
		// don't know about. The last resort is to get the value from the
		// controller at every n frames.
		//TSTR name;
		//cont->GetClassName(name);
		//TSTR className = FixupName(name);
		//Interface14 *iface = GetCOREInterface14();
		//UINT codepage = iface->DefaultTextSaveCodePage(true);
		//const char* className_locale = className.ToCP(codepage);
		//_ftprintf(pStream, _T("%s\t\t%s \"%hs\" {\n"), indent.data(), ID_CONTROL_FLOAT_SAMPLE,
		//	className_locale);

		//// If it is animated at all...
		//if (cont->IsAnimated()) {
		//	// Get the range of the controller animation
		//	Interval range;
		//	// Get range of full animation
		//	Interval animRange = ip->GetAnimRange();
		//	TimeValue t = cont->GetTimeRange(TIMERANGE_ALL).Start();
		//	float value;

		//	// While we are inside the animation...
		//	while (animRange.InInterval(t)) {
		//		// Sample the controller
		//		range = FOREVER;
		//		cont->GetValue(t, &value, range);

		//		// Set time to start of controller validity interval
		//		t = range.Start();

		//		// Output the sample
		//		_ftprintf(pStream, _T("%s\t\t\t%s %d\t%s\n"),
		//			indent.data(),
		//			ID_FLOAT_KEY,
		//			t,
		//			Format(value).data());

		//		// If the end of the controller validity is beyond the
		//		// range of the animation
		//		if (range.End() > cont->GetTimeRange(TIMERANGE_ALL).End()) {
		//			break;
		//		}
		//		else {
		//			t = (range.End() / GetTicksPerFrame() + GetKeyFrameStep()) * GetTicksPerFrame();
		//		}
		//	}
		//}
		//_ftprintf(pStream, _T("%s\t\t}\n"), indent.data());
	}
	return json_null;
}

/****************************************************************************

Inverse Kinematics (IK) Joint information

****************************************************************************/
json DumpJointParams(JointParams *joint);
json ExportIKJoints(INode *node)
{
	json dump(json::value_t::object);

	if (node->TestAFlag(A_INODE_IK_TERMINATOR))
		dump[ID_IKTERMINATOR] = true;

	if (node->TestAFlag(A_INODE_IK_POS_PINNED))
		dump[ID_IKPOS_PINNED] = true;

	if (node->TestAFlag(A_INODE_IK_ROT_PINNED))
		dump[ID_IKROT_PINNED] = true;

	// Position joint
	Control *cont = node->GetTMController()->GetPositionController();
	if (cont)
	{
		JointParams *joint = (JointParams *)cont->GetProperty(PROPID_JOINTPARAMS);
		if (joint && !joint->IsDefault())
		{
			// Has IK Joints!!!
			dump[ID_IKJOINT] = DumpJointParams(joint);
		}
	}

	// Rotational joint
	cont = node->GetTMController()->GetRotationController();
	if (cont)
	{
		JointParams *joint = (JointParams *)cont->GetProperty(PROPID_JOINTPARAMS);
		if (joint && !joint->IsDefault())
		{
			// Has IK Joints!!!
			dump[ID_IKJOINT] = DumpJointParams(joint);
		}
	}
	return dump;
}

json DumpJointParams(JointParams *joint)
{
	float scale = joint->scale;
	json dump =
		{
			{ID_IKTYPE, joint->Type() == JNT_POS ? ID_IKTYPEPOS : ID_IKTYPEROT},
			{ID_IKDOF, joint->dofs},
			{ID_IKXACTIVE, joint->flags & JNT_XACTIVE ? 1 : 0},
			{ID_IKYACTIVE, joint->flags & JNT_YACTIVE ? 1 : 0},
			{ID_IKZACTIVE, joint->flags & JNT_ZACTIVE ? 1 : 0},
			{ID_IKXLIMITED, joint->flags & JNT_XLIMITED ? 1 : 0},
			{ID_IKYLIMITED, joint->flags & JNT_YLIMITED ? 1 : 0},
			{ID_IKZLIMITED, joint->flags & JNT_ZLIMITED ? 1 : 0},
			{ID_IKXEASE, joint->flags & JNT_XEASE ? 1 : 0},
			{ID_IKYEASE, joint->flags & JNT_YEASE ? 1 : 0},
			{ID_IKZEASE, joint->flags & JNT_ZEASE ? 1 : 0},
			{ID_IKLIMITEXACT, joint->flags & JNT_LIMITEXACT ? 1 : 0}};

	std::vector<json> jointInfo;
	for (int i = 0; i < joint->dofs; i++)
	{
		jointInfo.push_back({{_T("min"), Format(joint->min[i])},
							 {_T("max"), Format(joint->max[i])},
							 {_T("dmaping"), Format(joint->damping[i])}});
	}
	dump[ID_IKJOINTINFO] = jointInfo;
	return dump;
}

void ExportGeomObject(INode *node, json &dump)
{
	auto obj = node->EvalWorldState(0).obj;
	// Targets are actually geomobjects, but we will export them
	// from the camera and light objects, so we skip them here.
	if (obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
		return;

	ExportNodeHeader(node, ID_GEOMETRY, dump);
	dump[ID_NODE_TM] = ExportNodeTM(node);

	/*if (GetIncludeMesh()) {
		ExportMesh(node, GetStaticFrame(), indentLevel);
	}*/

	// Node properties (only for geomobjects)
	dump[ID_PROP_MOTIONBLUR] = node->MotBlur();
	dump[ID_PROP_CASTSHADOW] = node->CastShadows();
	dump[ID_PROP_RECVSHADOW] = node->RcvShadows();

	ExportAnimKeys(node);

	// Export the visibility track
	Control *visCont = node->GetVisController();
	if (visCont)
	{
		dump[ID_VISIBILITY_TRACK] = DumpFloatKeys(visCont);
	}

	//if (GetIncludeMtl()) {
	//	ExportMaterial(node, indentLevel);
	//}

	//if (GetIncludeMeshAnim()) {
	//	ExportAnimMesh(node, indentLevel);
	//}

	if (GetIncludeIKJoints())
	{
		dump[_T("IKJoints")] = ExportIKJoints(node);
	}
}

json dumpNode(INode *node)
{
	json dump(json::value_t::object);
	dump[_T("selected")] = node->Selected();

	MSTR className;
	node->GetClassName(className);
	dump[_T("nodeClass")] = className;

	if (node->IsGroupHead())
	{
		TSTR nodeName = FixupName(node->GetName());
		dump[ID_NODE_NAME] = nodeName;
	}

	ObjectState os = node->EvalWorldState(0);
	// The obj member of ObjectState is the actual object we will export.
	if (os.obj)
	{
		// We look at the super class ID to determine the type of the object.
		switch (os.obj->SuperClassID())
		{
		case GEOMOBJECT_CLASS_ID:
			ExportGeomObject(node, dump);
			break;
			/*case CAMERA_CLASS_ID:
				ExportCameraObject(node);
				break;
			case LIGHT_CLASS_ID:
				ExportLightObject(node);
				break;
			case SHAPE_CLASS_ID:
				ExportShapeObject(node);
				break;
			case HELPER_CLASS_ID:
				ExportHelperObject(node);
				break;*/
		}
	}

	// dump children
	std::vector<json> childNodes;
	for (int c = 0; c < node->NumberOfChildren(); c++)
	{
		childNodes.push_back(dumpNode(node->GetChildNode(c)));
	}
	dump[_T("childNodes")] = childNodes;
	return dump;
}

json ExportKernel::export2JSON(Interface *ip)
{
	json dump(json::value_t::object);
	dump[_T("globalInfo")] = ExportGlobalInfo(ip);
	dump[_T("rootNode")] = dumpNode(ip->GetRootNode());
	// Export list of material definitions
	// ExportMaterialList();
	return dump;
}
