// Disable warnings related to using stl from modules and GTest from headers
#pragma warning(disable: 5050 4005)

#include "gtest/gtest.h"

import StdCore;
import Song;
import SongMetadata;
import SongMetadataComputations;
import Chart;
import ChartInfo;
import PlayStyle;
import Difficulty;
import NoteLength;
import NotePos;
import NoteRow;
import NoSuchChartException;
import ParseException;

using namespace DanceCommon;
using Event = SongMetadataComputations::Event;

static const double Epsilon = 0.00001;
static const double TimeEpsilon = 0.001;

static Song GetVertexDelta()
{
	std::ifstream stream{ "Vertex_Delta.sm" };
	return Song::Load(stream);
}

static Song GetSilikon()
{
	std::ifstream stream{ "silikon.sm" };
	return Song::Load(stream);
}

static std::vector<std::pair<double, Event>> GetExpectedBpms()
{
	std::vector<std::pair<double, Event>> expectedBpms;
	expectedBpms.push_back(std::pair(0.140000, Event(0, 111.000000)));
	expectedBpms.push_back(std::pair(16.886243, Event(1176, 55.500000)));
	expectedBpms.push_back(std::pair(17.426784, Event(1200, 111.000000)));
	expectedBpms.push_back(std::pair(34.162243, Event(2040, 55.500000)));
	expectedBpms.push_back(std::pair(34.702784, Event(2064, 111.000000)));
	expectedBpms.push_back(std::pair(42.408270, Event(2640, 222.000000)));
	expectedBpms.push_back(std::pair(43.353811, Event(2736, 111.000000)));
	expectedBpms.push_back(std::pair(50.990568, Event(3336, 222.000000)));
	expectedBpms.push_back(std::pair(51.192703, Event(3360, 111.000000)));
	expectedBpms.push_back(std::pair(51.462973, Event(3384, 55.500000)));
	expectedBpms.push_back(std::pair(52.003514, Event(3408, 111.000000)));
	expectedBpms.push_back(std::pair(59.571081, Event(4080, 55.500000)));
	expectedBpms.push_back(std::pair(60.652162, Event(4128, 111.000000)));
	expectedBpms.push_back(std::pair(65.044486, Event(4512, 222.000000)));
	expectedBpms.push_back(std::pair(68.750378, Event(4848, 55.500000)));
	expectedBpms.push_back(std::pair(69.290919, Event(4872, 111.000000)));
	expectedBpms.push_back(std::pair(71.504081, Event(5064, 444.000000)));
	expectedBpms.push_back(std::pair(71.588865, Event(5076, 111.000000)));
	expectedBpms.push_back(std::pair(71.910135, Event(5100, 444.000000)));
	expectedBpms.push_back(std::pair(71.994919, Event(5112, 111.000000)));
	expectedBpms.push_back(std::pair(72.586459, Event(5160, 444.000000)));
	expectedBpms.push_back(std::pair(72.671243, Event(5172, 111.000000)));
	expectedBpms.push_back(std::pair(72.992514, Event(5196, 444.000000)));
	expectedBpms.push_back(std::pair(73.077297, Event(5208, 111.000000)));
	expectedBpms.push_back(std::pair(75.831000, Event(5448, 444.000000)));
	expectedBpms.push_back(std::pair(75.915784, Event(5460, 111.000000)));
	expectedBpms.push_back(std::pair(76.237054, Event(5484, 444.000000)));
	expectedBpms.push_back(std::pair(76.321838, Event(5496, 111.000000)));
	expectedBpms.push_back(std::pair(76.913378, Event(5544, 444.000000)));
	expectedBpms.push_back(std::pair(76.998162, Event(5556, 111.000000)));
	expectedBpms.push_back(std::pair(77.319432, Event(5580, 444.000000)));
	expectedBpms.push_back(std::pair(77.404216, Event(5592, 111.000000)));
	expectedBpms.push_back(std::pair(80.157378, Event(5784, 444.000000)));
	expectedBpms.push_back(std::pair(80.242162, Event(5796, 111.000000)));
	expectedBpms.push_back(std::pair(80.563432, Event(5820, 444.000000)));
	expectedBpms.push_back(std::pair(80.648216, Event(5832, 111.000000)));
	expectedBpms.push_back(std::pair(81.239757, Event(5880, 444.000000)));
	expectedBpms.push_back(std::pair(81.324541, Event(5892, 111.000000)));
	expectedBpms.push_back(std::pair(81.645811, Event(5916, 444.000000)));
	expectedBpms.push_back(std::pair(81.730595, Event(5928, 111.000000)));
	expectedBpms.push_back(std::pair(85.919568, Event(6192, 222.000000)));
	expectedBpms.push_back(std::pair(86.054703, Event(6216, 55.500000)));
	expectedBpms.push_back(std::pair(86.595243, Event(6240, 111.000000)));
	expectedBpms.push_back(std::pair(103.231162, Event(7032, 222.000000)));
	expectedBpms.push_back(std::pair(103.366297, Event(7056, 55.500000)));
	expectedBpms.push_back(std::pair(103.906838, Event(7080, 111.000000)));
	expectedBpms.push_back(std::pair(111.611595, Event(7680, 222.000000)));
	expectedBpms.push_back(std::pair(112.557135, Event(7776, 111.000000)));
	expectedBpms.push_back(std::pair(120.126892, Event(8376, 55.500000)));
	expectedBpms.push_back(std::pair(121.207973, Event(8424, 111.000000)));
	expectedBpms.push_back(std::pair(128.910541, Event(9096, 222.000000)));
	expectedBpms.push_back(std::pair(129.856081, Event(9192, 111.000000)));
	expectedBpms.push_back(std::pair(136.477568, Event(9768, 222.000000)));
	expectedBpms.push_back(std::pair(137.963378, Event(9912, 55.500000)));
	expectedBpms.push_back(std::pair(138.503919, Event(9936, 111.000000)));
	expectedBpms.push_back(std::pair(145.328405, Event(10512, 222.000000)));

	return expectedBpms;
}


static std::vector<std::pair<double, Event>> GetExpectedStops()
{
	std::vector<std::pair<double, Event>> expectedStops;
	expectedStops.push_back(std::pair(9.464324, Event(828, 0.135000)));
	expectedStops.push_back(std::pair(9.734459, Event(840, 0.135000)));
	expectedStops.push_back(std::pair(10.410000, Event(888, 0.405000)));
	expectedStops.push_back(std::pair(10.950135, Event(900, 0.135000)));
	expectedStops.push_back(std::pair(11.625676, Event(948, 0.270000)));
	expectedStops.push_back(std::pair(12.030811, Event(960, 0.135000)));
	expectedStops.push_back(std::pair(12.300946, Event(972, 0.135000)));
	expectedStops.push_back(std::pair(12.571081, Event(984, 0.135000)));
	expectedStops.push_back(std::pair(12.841216, Event(996, 0.135000)));
	expectedStops.push_back(std::pair(13.111351, Event(1008, 0.135000)));
	expectedStops.push_back(std::pair(13.381486, Event(1020, 0.067000)));
	expectedStops.push_back(std::pair(13.516054, Event(1026, 0.067000)));
	expectedStops.push_back(std::pair(13.650622, Event(1032, 0.067000)));
	expectedStops.push_back(std::pair(13.785189, Event(1038, 0.067000)));
	expectedStops.push_back(std::pair(13.919757, Event(1044, 0.067000)));
	expectedStops.push_back(std::pair(14.054324, Event(1050, 0.067000)));
	expectedStops.push_back(std::pair(14.188892, Event(1056, 0.067000)));
	expectedStops.push_back(std::pair(14.323459, Event(1062, 0.067000)));
	expectedStops.push_back(std::pair(14.458027, Event(1068, 0.067000)));
	expectedStops.push_back(std::pair(14.592595, Event(1074, 0.067000)));
	expectedStops.push_back(std::pair(14.727162, Event(1080, 0.067000)));
	expectedStops.push_back(std::pair(14.861730, Event(1086, 0.067000)));
	expectedStops.push_back(std::pair(14.996297, Event(1092, 0.067000)));
	expectedStops.push_back(std::pair(15.130865, Event(1098, 0.067000)));
	expectedStops.push_back(std::pair(15.265432, Event(1104, 0.135000)));
	expectedStops.push_back(std::pair(15.535568, Event(1116, 0.135000)));
	expectedStops.push_back(std::pair(15.805703, Event(1128, 0.135000)));
	expectedStops.push_back(std::pair(16.075838, Event(1140, 0.135000)));
	expectedStops.push_back(std::pair(16.345973, Event(1152, 0.135000)));
	expectedStops.push_back(std::pair(16.616108, Event(1164, 0.135000)));
	expectedStops.push_back(std::pair(18.102459, Event(1260, 0.135000)));
	expectedStops.push_back(std::pair(18.372595, Event(1272, 0.135000)));
	expectedStops.push_back(std::pair(19.048135, Event(1320, 0.405000)));
	expectedStops.push_back(std::pair(19.588270, Event(1332, 0.135000)));
	expectedStops.push_back(std::pair(20.263811, Event(1380, 0.270000)));
	expectedStops.push_back(std::pair(20.668946, Event(1392, 0.135000)));
	expectedStops.push_back(std::pair(20.939081, Event(1404, 0.135000)));
	expectedStops.push_back(std::pair(21.209216, Event(1416, 0.135000)));
	expectedStops.push_back(std::pair(21.479351, Event(1428, 0.135000)));
	expectedStops.push_back(std::pair(21.749486, Event(1440, 0.135000)));
	expectedStops.push_back(std::pair(22.019622, Event(1452, 0.067000)));
	expectedStops.push_back(std::pair(22.154189, Event(1458, 0.067000)));
	expectedStops.push_back(std::pair(22.288757, Event(1464, 0.067000)));
	expectedStops.push_back(std::pair(22.423324, Event(1470, 0.067000)));
	expectedStops.push_back(std::pair(22.557892, Event(1476, 0.067000)));
	expectedStops.push_back(std::pair(22.692459, Event(1482, 0.067000)));
	expectedStops.push_back(std::pair(22.827027, Event(1488, 0.067000)));
	expectedStops.push_back(std::pair(22.961595, Event(1494, 0.067000)));
	expectedStops.push_back(std::pair(23.096162, Event(1500, 0.067000)));
	expectedStops.push_back(std::pair(23.230730, Event(1506, 0.067000)));
	expectedStops.push_back(std::pair(23.365297, Event(1512, 0.067000)));
	expectedStops.push_back(std::pair(23.499865, Event(1518, 0.067000)));
	expectedStops.push_back(std::pair(23.634432, Event(1524, 0.067000)));
	expectedStops.push_back(std::pair(23.769000, Event(1530, 0.067000)));
	expectedStops.push_back(std::pair(23.903568, Event(1536, 0.135000)));
	expectedStops.push_back(std::pair(24.173703, Event(1548, 0.135000)));
	expectedStops.push_back(std::pair(24.443838, Event(1560, 0.135000)));
	expectedStops.push_back(std::pair(24.713973, Event(1572, 0.135000)));
	expectedStops.push_back(std::pair(24.984108, Event(1584, 0.135000)));
	expectedStops.push_back(std::pair(25.254243, Event(1596, 0.135000)));
	expectedStops.push_back(std::pair(25.524378, Event(1608, 0.270000)));
	expectedStops.push_back(std::pair(26.740324, Event(1692, 0.135000)));
	expectedStops.push_back(std::pair(27.010459, Event(1704, 0.135000)));
	expectedStops.push_back(std::pair(27.686000, Event(1752, 0.405000)));
	expectedStops.push_back(std::pair(28.226135, Event(1764, 0.135000)));
	expectedStops.push_back(std::pair(28.901676, Event(1812, 0.270000)));
	expectedStops.push_back(std::pair(29.306811, Event(1824, 0.135000)));
	expectedStops.push_back(std::pair(29.576946, Event(1836, 0.135000)));
	expectedStops.push_back(std::pair(29.847081, Event(1848, 0.135000)));
	expectedStops.push_back(std::pair(30.117216, Event(1860, 0.135000)));
	expectedStops.push_back(std::pair(30.387351, Event(1872, 0.135000)));
	expectedStops.push_back(std::pair(30.657486, Event(1884, 0.067000)));
	expectedStops.push_back(std::pair(30.792054, Event(1890, 0.067000)));
	expectedStops.push_back(std::pair(30.926622, Event(1896, 0.067000)));
	expectedStops.push_back(std::pair(31.061189, Event(1902, 0.067000)));
	expectedStops.push_back(std::pair(31.195757, Event(1908, 0.067000)));
	expectedStops.push_back(std::pair(31.330324, Event(1914, 0.067000)));
	expectedStops.push_back(std::pair(31.464892, Event(1920, 0.067000)));
	expectedStops.push_back(std::pair(31.599459, Event(1926, 0.067000)));
	expectedStops.push_back(std::pair(31.734027, Event(1932, 0.067000)));
	expectedStops.push_back(std::pair(31.868595, Event(1938, 0.067000)));
	expectedStops.push_back(std::pair(32.003162, Event(1944, 0.067000)));
	expectedStops.push_back(std::pair(32.137730, Event(1950, 0.067000)));
	expectedStops.push_back(std::pair(32.272297, Event(1956, 0.067000)));
	expectedStops.push_back(std::pair(32.406865, Event(1962, 0.067000)));
	expectedStops.push_back(std::pair(32.541432, Event(1968, 0.135000)));
	expectedStops.push_back(std::pair(32.811568, Event(1980, 0.135000)));
	expectedStops.push_back(std::pair(33.081703, Event(1992, 0.135000)));
	expectedStops.push_back(std::pair(33.351838, Event(2004, 0.135000)));
	expectedStops.push_back(std::pair(33.621973, Event(2016, 0.135000)));
	expectedStops.push_back(std::pair(33.892108, Event(2028, 0.135000)));
	expectedStops.push_back(std::pair(35.648730, Event(2148, 0.271000)));
	expectedStops.push_back(std::pair(37.811622, Event(2316, 0.271000)));
	expectedStops.push_back(std::pair(39.974514, Event(2484, 0.271000)));
	expectedStops.push_back(std::pair(41.867135, Event(2628, 0.271000)));
	expectedStops.push_back(std::pair(42.273270, Event(2640, 0.135000)));
	expectedStops.push_back(std::pair(42.543405, Event(2664, 0.135000)));
	expectedStops.push_back(std::pair(42.813541, Event(2688, 0.135000)));
	expectedStops.push_back(std::pair(43.083676, Event(2712, 0.135000)));
	expectedStops.push_back(std::pair(44.299757, Event(2820, 0.271000)));
	expectedStops.push_back(std::pair(46.462649, Event(2988, 0.271000)));
	expectedStops.push_back(std::pair(48.625541, Event(3156, 0.271000)));
	expectedStops.push_back(std::pair(50.923568, Event(3336, 0.067000)));
	expectedStops.push_back(std::pair(51.058135, Event(3348, 0.067000)));
	expectedStops.push_back(std::pair(64.976486, Event(4512, 0.068000)));
	expectedStops.push_back(std::pair(65.112054, Event(4524, 0.068000)));
	expectedStops.push_back(std::pair(65.247622, Event(4536, 0.068000)));
	expectedStops.push_back(std::pair(65.383189, Event(4548, 0.068000)));
	expectedStops.push_back(std::pair(65.518757, Event(4560, 0.068000)));
	expectedStops.push_back(std::pair(65.654324, Event(4572, 0.067000)));
	expectedStops.push_back(std::pair(65.788892, Event(4584, 0.067000)));
	expectedStops.push_back(std::pair(65.923459, Event(4596, 0.067000)));
	expectedStops.push_back(std::pair(66.058027, Event(4608, 0.067000)));
	expectedStops.push_back(std::pair(66.192595, Event(4620, 0.067000)));
	expectedStops.push_back(std::pair(66.327162, Event(4632, 0.067000)));
	expectedStops.push_back(std::pair(66.461730, Event(4644, 0.067000)));
	expectedStops.push_back(std::pair(66.596297, Event(4656, 0.067000)));
	expectedStops.push_back(std::pair(66.730865, Event(4668, 0.067000)));
	expectedStops.push_back(std::pair(66.865432, Event(4680, 0.067000)));
	expectedStops.push_back(std::pair(67.000000, Event(4692, 0.067000)));
	expectedStops.push_back(std::pair(67.134568, Event(4704, 0.067000)));
	expectedStops.push_back(std::pair(67.269135, Event(4716, 0.067000)));
	expectedStops.push_back(std::pair(67.403703, Event(4728, 0.067000)));
	expectedStops.push_back(std::pair(67.538270, Event(4740, 0.067000)));
	expectedStops.push_back(std::pair(67.672838, Event(4752, 0.067000)));
	expectedStops.push_back(std::pair(67.807405, Event(4764, 0.067000)));
	expectedStops.push_back(std::pair(67.941973, Event(4776, 0.067000)));
	expectedStops.push_back(std::pair(68.076541, Event(4788, 0.067000)));
	expectedStops.push_back(std::pair(68.211108, Event(4800, 0.067000)));
	expectedStops.push_back(std::pair(68.345676, Event(4812, 0.067000)));
	expectedStops.push_back(std::pair(68.480243, Event(4824, 0.135000)));
	expectedStops.push_back(std::pair(71.453081, Event(5064, 0.051000)));
	expectedStops.push_back(std::pair(71.520973, Event(5070, 0.051000)));
	expectedStops.push_back(std::pair(71.859135, Event(5100, 0.051000)));
	expectedStops.push_back(std::pair(71.927027, Event(5106, 0.051000)));
	expectedStops.push_back(std::pair(72.535459, Event(5160, 0.051000)));
	expectedStops.push_back(std::pair(72.603351, Event(5166, 0.051000)));
	expectedStops.push_back(std::pair(72.941514, Event(5196, 0.051000)));
	expectedStops.push_back(std::pair(73.009405, Event(5202, 0.051000)));
	expectedStops.push_back(std::pair(75.780000, Event(5448, 0.051000)));
	expectedStops.push_back(std::pair(75.847892, Event(5454, 0.051000)));
	expectedStops.push_back(std::pair(76.186054, Event(5484, 0.051000)));
	expectedStops.push_back(std::pair(76.253946, Event(5490, 0.051000)));
	expectedStops.push_back(std::pair(76.862378, Event(5544, 0.051000)));
	expectedStops.push_back(std::pair(76.930270, Event(5550, 0.051000)));
	expectedStops.push_back(std::pair(77.268432, Event(5580, 0.051000)));
	expectedStops.push_back(std::pair(77.336324, Event(5586, 0.051000)));
	expectedStops.push_back(std::pair(78.890703, Event(5724, 0.135000)));
	expectedStops.push_back(std::pair(79.160838, Event(5736, 0.135000)));
	expectedStops.push_back(std::pair(79.430973, Event(5748, 0.135000)));
	expectedStops.push_back(std::pair(79.701108, Event(5760, 0.135000)));
	expectedStops.push_back(std::pair(80.106378, Event(5784, 0.051000)));
	expectedStops.push_back(std::pair(80.174270, Event(5790, 0.051000)));
	expectedStops.push_back(std::pair(80.512432, Event(5820, 0.051000)));
	expectedStops.push_back(std::pair(80.580324, Event(5826, 0.051000)));
	expectedStops.push_back(std::pair(81.188757, Event(5880, 0.051000)));
	expectedStops.push_back(std::pair(81.256649, Event(5886, 0.051000)));
	expectedStops.push_back(std::pair(81.594811, Event(5916, 0.051000)));
	expectedStops.push_back(std::pair(81.662703, Event(5922, 0.051000)));
	expectedStops.push_back(std::pair(83.217081, Event(6060, 0.135000)));
	expectedStops.push_back(std::pair(84.027757, Event(6120, 0.067000)));
	expectedStops.push_back(std::pair(84.162324, Event(6126, 0.067000)));
	expectedStops.push_back(std::pair(84.296892, Event(6132, 0.067000)));
	expectedStops.push_back(std::pair(84.431459, Event(6138, 0.205000)));
	expectedStops.push_back(std::pair(84.704027, Event(6144, 0.135000)));
	expectedStops.push_back(std::pair(84.974162, Event(6156, 0.135000)));
	expectedStops.push_back(std::pair(85.244297, Event(6168, 0.135000)));
	expectedStops.push_back(std::pair(85.514432, Event(6180, 0.135000)));
	expectedStops.push_back(std::pair(85.784568, Event(6192, 0.135000)));
	expectedStops.push_back(std::pair(87.270919, Event(6300, 0.135000)));
	expectedStops.push_back(std::pair(87.541054, Event(6312, 0.135000)));
	expectedStops.push_back(std::pair(88.216595, Event(6360, 0.405000)));
	expectedStops.push_back(std::pair(88.756730, Event(6372, 0.135000)));
	expectedStops.push_back(std::pair(89.432270, Event(6420, 0.270000)));
	expectedStops.push_back(std::pair(89.837405, Event(6432, 0.135000)));
	expectedStops.push_back(std::pair(90.107541, Event(6444, 0.135000)));
	expectedStops.push_back(std::pair(90.377676, Event(6456, 0.135000)));
	expectedStops.push_back(std::pair(90.647811, Event(6468, 0.135000)));
	expectedStops.push_back(std::pair(90.917946, Event(6480, 0.135000)));
	expectedStops.push_back(std::pair(91.188081, Event(6492, 0.068000)));
	expectedStops.push_back(std::pair(91.323649, Event(6498, 0.068000)));
	expectedStops.push_back(std::pair(91.459216, Event(6504, 0.068000)));
	expectedStops.push_back(std::pair(91.594784, Event(6510, 0.068000)));
	expectedStops.push_back(std::pair(91.730351, Event(6516, 0.068000)));
	expectedStops.push_back(std::pair(91.865919, Event(6522, 0.067000)));
	expectedStops.push_back(std::pair(92.000486, Event(6528, 0.067000)));
	expectedStops.push_back(std::pair(92.135054, Event(6534, 0.067000)));
	expectedStops.push_back(std::pair(92.269622, Event(6540, 0.067000)));
	expectedStops.push_back(std::pair(92.404189, Event(6546, 0.067000)));
	expectedStops.push_back(std::pair(92.538757, Event(6552, 0.067000)));
	expectedStops.push_back(std::pair(92.673324, Event(6558, 0.067000)));
	expectedStops.push_back(std::pair(92.807892, Event(6564, 0.067000)));
	expectedStops.push_back(std::pair(92.942459, Event(6570, 0.067000)));
	expectedStops.push_back(std::pair(93.077027, Event(6576, 0.135000)));
	expectedStops.push_back(std::pair(93.347162, Event(6588, 0.135000)));
	expectedStops.push_back(std::pair(93.617297, Event(6600, 0.135000)));
	expectedStops.push_back(std::pair(93.887432, Event(6612, 0.135000)));
	expectedStops.push_back(std::pair(94.157568, Event(6624, 0.135000)));
	expectedStops.push_back(std::pair(94.427703, Event(6636, 0.135000)));
	expectedStops.push_back(std::pair(94.697838, Event(6648, 0.275000)));
	expectedStops.push_back(std::pair(95.918784, Event(6732, 0.135000)));
	expectedStops.push_back(std::pair(96.188919, Event(6744, 0.135000)));
	expectedStops.push_back(std::pair(96.864459, Event(6792, 0.405000)));
	expectedStops.push_back(std::pair(97.404595, Event(6804, 0.135000)));
	expectedStops.push_back(std::pair(98.080135, Event(6852, 0.270000)));
	expectedStops.push_back(std::pair(98.485270, Event(6864, 0.135000)));
	expectedStops.push_back(std::pair(98.755405, Event(6876, 0.135000)));
	expectedStops.push_back(std::pair(99.025541, Event(6888, 0.135000)));
	expectedStops.push_back(std::pair(99.295676, Event(6900, 0.135000)));
	expectedStops.push_back(std::pair(99.565811, Event(6912, 0.135000)));
	expectedStops.push_back(std::pair(99.835946, Event(6924, 0.068000)));
	expectedStops.push_back(std::pair(99.971514, Event(6930, 0.068000)));
	expectedStops.push_back(std::pair(100.107081, Event(6936, 0.068000)));
	expectedStops.push_back(std::pair(100.242649, Event(6942, 0.068000)));
	expectedStops.push_back(std::pair(100.378216, Event(6948, 0.068000)));
	expectedStops.push_back(std::pair(100.513784, Event(6954, 0.067000)));
	expectedStops.push_back(std::pair(100.648351, Event(6960, 0.067000)));
	expectedStops.push_back(std::pair(100.782919, Event(6966, 0.067000)));
	expectedStops.push_back(std::pair(100.917486, Event(6972, 0.067000)));
	expectedStops.push_back(std::pair(101.052054, Event(6978, 0.067000)));
	expectedStops.push_back(std::pair(101.186622, Event(6984, 0.067000)));
	expectedStops.push_back(std::pair(101.321189, Event(6990, 0.067000)));
	expectedStops.push_back(std::pair(101.455757, Event(6996, 0.067000)));
	expectedStops.push_back(std::pair(101.590324, Event(7002, 0.067000)));
	expectedStops.push_back(std::pair(101.724892, Event(7008, 0.410000)));
	expectedStops.push_back(std::pair(102.270027, Event(7020, 0.286000)));
	expectedStops.push_back(std::pair(102.691162, Event(7032, 0.540000)));
	expectedStops.push_back(std::pair(104.852784, Event(7164, 0.271000)));
	expectedStops.push_back(std::pair(107.015676, Event(7332, 0.271000)));
	expectedStops.push_back(std::pair(109.178568, Event(7500, 0.271000)));
	expectedStops.push_back(std::pair(111.476595, Event(7680, 0.135000)));
	expectedStops.push_back(std::pair(111.746730, Event(7704, 0.135000)));
	expectedStops.push_back(std::pair(112.016865, Event(7728, 0.135000)));
	expectedStops.push_back(std::pair(112.287000, Event(7752, 0.135000)));
	expectedStops.push_back(std::pair(113.503081, Event(7860, 0.271000)));
	expectedStops.push_back(std::pair(115.665973, Event(8028, 0.271000)));
	expectedStops.push_back(std::pair(117.828865, Event(8196, 0.271000)));
	expectedStops.push_back(std::pair(128.775541, Event(9096, 0.135000)));
	expectedStops.push_back(std::pair(129.045676, Event(9120, 0.135000)));
	expectedStops.push_back(std::pair(129.315811, Event(9144, 0.135000)));
	expectedStops.push_back(std::pair(129.585946, Event(9168, 0.135000)));
	expectedStops.push_back(std::pair(136.342568, Event(9768, 0.135000)));
	expectedStops.push_back(std::pair(136.612703, Event(9792, 0.135000)));
	expectedStops.push_back(std::pair(136.882838, Event(9816, 0.135000)));
	expectedStops.push_back(std::pair(137.152973, Event(9840, 0.135000)));
	expectedStops.push_back(std::pair(137.423108, Event(9864, 0.135000)));
	expectedStops.push_back(std::pair(137.693243, Event(9888, 0.135000)));
	expectedStops.push_back(std::pair(144.990405, Event(10512, 0.338000)));
	expectedStops.push_back(std::pair(145.395973, Event(10524, 0.338000)));
	expectedStops.push_back(std::pair(145.801541, Event(10536, 0.473000)));
	expectedStops.push_back(std::pair(146.342108, Event(10548, 0.203000)));
	expectedStops.push_back(std::pair(146.612676, Event(10560, 0.136000)));
	return expectedStops;
}

TEST(SongMetadataComputations, GetEvents)
{
	Song song = GetSilikon();
	auto metadata = song.GetMetadata();
	auto computations = metadata.GetComputations();

	auto bpmsByTime = computations.GetBpmsByTime();
	auto expectedBpms = GetExpectedBpms();
	
	size_t i = 0;
	for (auto& actual : bpmsByTime)
	{
		auto& expected = expectedBpms[expectedBpms.size() - i - 1];
		EXPECT_NEAR(expected.first, actual.first, Epsilon);
		EXPECT_EQ(expected.second, actual.second);
		i++;
	}

	auto stopsByTime = computations.GetStopsByTime();
	auto expectedStops = GetExpectedStops();

	i = 0;
	for (auto& actual : stopsByTime)
	{
		auto& expected = expectedStops[expectedStops.size() - i - 1];
		EXPECT_NEAR(expected.first, actual.first, Epsilon);
		EXPECT_EQ(expected.second, actual.second);
		i++;
	}
}
