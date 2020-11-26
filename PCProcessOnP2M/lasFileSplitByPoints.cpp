#include<iostream>
#include<lasreader.hpp>
#include<laswriter.hpp>
#include <sstream>

using namespace std;

void lasFileSplitByPoints()
{
	LASreadOpener lasreadopener;
	LASwriteOpener laswriteopener;
	//laslib只允许'\\'格式的文件路径。
	std::string lasFile("D:\\1.las");

	int num = 1;
	//打开las文件
	lasreadopener.set_file_name(lasFile.c_str());
	LASreader* lasreader = lasreadopener.open();
	size_t count = lasreader->header.number_of_point_records;
	printf("%d", count);
	//LASpoint *one_point = new LASpoint();
	int loop_time = 0;
	LASwriter* laswriter = nullptr;
	while (lasreader->read_point())  //只让输出前 10 行坐标
	{
		//按照每889000个点一个las文件进行分割
		if (loop_time % 20000 == 0)
		{ 
			//每打开一次文件都需要close();否则文件会一直占用。
			if (laswriter != NULL)
			{
				laswriter->close();
			}
			stringstream ss; 
			ss << "D:\\out" << num << ".las";
			string file_name = ss.str();
			laswriteopener.set_file_name(file_name.c_str());

			LASheader writeHeader;
			writeHeader.number_of_point_records = 20000;

			writeHeader.min_x = lasreader->header.min_x;
			writeHeader.max_x = lasreader->header.max_x;
			writeHeader.min_y = lasreader->header.min_y;
			writeHeader.max_y = lasreader->header.max_y;
			writeHeader.min_z = lasreader->header.min_z;
			writeHeader.max_z = lasreader->header.max_z;
			writeHeader.x_scale_factor = lasreader->header.x_scale_factor;
			writeHeader.y_scale_factor = lasreader->header.y_scale_factor;;
			writeHeader.z_scale_factor = lasreader->header.z_scale_factor;;
			writeHeader.x_offset = lasreader->header.x_offset;
			writeHeader.y_offset = lasreader->header.y_offset;
			writeHeader.z_offset = lasreader->header.z_offset;
			writeHeader.point_data_format = 1;
			writeHeader.point_data_record_length = 28;

			laswriter = laswriteopener.open(&writeHeader);
			if (laswriter == 0)
			{
				fprintf(stderr, "ERROR: could not open laswriter\n");
			}
			num++;
		}
		laswriter->write_point(&lasreader->point);
		laswriter->update_inventory(&lasreader->point);
		loop_time++;
	}
	laswriter->update_header(&lasreader->header, TRUE);

	I64 total_bytes = laswriter->close();
	delete laswriter;
	laswriter = nullptr;

	lasreader->close();
	delete lasreader;
	lasreader = nullptr;
}

//void main()
//{
//	lasFileSplitByPoints();
//}