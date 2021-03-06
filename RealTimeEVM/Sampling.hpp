/**
* Real Time non-envasive heartbeat reader using "Eulerian Video Magnification" algorithm.
* Uses the OpenCV library and Microsoft Kinect One
* Based on the work of:
*      Jens Schindel		<kontakt@jens-schindel.de>
*      Joseph Pan			<https://github.com/wzpan/QtEVM>
*      Nick D'Ademo			<https://github.com/nickdademo/qt-opencv-multithreaded>
* @author Francisco Salgado <salgfrancisco@gmail.com>
* @version: 1.0
*/

#ifndef SAMPLING_HPP
#define SAMPLING_HPP

namespace sampling {
	const int faceTopPoint = 29;
	const int faceLeftPoint = 36;
	const int faceBottomPoint = 0;
	const int faceRightPoints = 614;

	const int samplingPoints[] = {
		28,
		29,
		30,
		53,
		54,
		55,
		56,
		73,
		74,
		87,
		88,
		96,
		100,
		101,
		111,
		112,
		115,
		122,
		124,
		145,
		156,
		239,
		246,
		247,
		248,
		249,
		250,
		251,
		252,
		253,
		254,
		255,
		256,
		266,
		268,
		269,
		274,
		277,
		278,
		281,
		282,
		289,
		290,
		291,
		295,
		296,
		299,
		300,
		301,
		302,
		303,
		304,
		305,
		351,
		352,
		353,
		354,
		412,
		413,
		414,
		415,
		420,
		436,
		437,
		438,
		443,
		444,
		445,
		450,
		451,
		458,
		464,
		465,
		466,
		472,
		473,
		474,
		552,
		590,
		639,
		643,
		672,
		674,
		675,
		676,
		677,
		692,
		694,
		695,
		696,
		697,
		717,
		718,
		719,
		720,
		734,
		735,
		736,
		737,
		738,
		739,
		757,
		763,
		764,
		879,
		880,
		881,
		884,
		885,
		886,
		887,
		888,
		889,
		890,
		891,
		892,
		893,
		894,
		899,
		913,
		921,
		922,
		923,
		924,
		926,
		927,
		928,
		929,
		930,
		931,
		932,
		933,
		934,
		935,
		936,
		939,
		940,
		946,
		947,
		948,
		949,
		950,
		951,
		958,
		983,
		1002,
		1005,
		1014,
		1018,
		1019,
		1049,
		1052,
		1067,
		1068,
		1069,
		1156,
		1157,
		1158,
		1174,
		1175,
		1176,
		1198,
		1199,
		1200,
		1210,
		1211,
		1220,
		1240,
		1241,
		1242,
		1243,
		1248,
		1255,
		1256,
		1257,
		1258,
		1260,
		1269,
		1271,
		1277,
		1279,
		1281,
		1282,
		1286,
		1290,
	};
}

#endif