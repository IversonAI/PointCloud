#include<iostream>
#include<lasreader.hpp>
#include<laswriter.hpp>
#include <sstream>
#include <fstream>
#include<vector>
using namespace std;

//size_t GetFileSize(const std::string& file_name) {
//	std::ifstream in(file_name.c_str());
//	in.seekg(0, std::ios::end);
//	size_t size = in.tellg();
//	in.close();
//	return size; //单位是：byte
//}
//
////const string &filepath
//void lasFileSpiltBySize1(const string &filepath, int blockSize)
//{
//	LASreadOpener lasreadopener;
//	LASwriteOpener laswriteopener;
//
//	int FileSize = GetFileSize(filepath);
//	blockSize = blockSize * 1024; //MB -> B
//	int chunk = FileSize / blockSize + 1;
//	cout << chunk << endl;
//	//打开las文件
//	lasreadopener.set_file_name(filepath.c_str());
//	LASreader* lasreader = lasreadopener.open();
//	size_t count = lasreader->header.number_of_point_records;
//	printf("%d\n", count);
//
//	int num = 1;
//	//LASpoint *one_point = new LASpoint();
//	int loop_time = 0;
//	LASwriter* laswriter = nullptr;
//	while (lasreader->read_point())
//	{
//		if (count%chunk == 0)
//		{
//			if (loop_time % (count / chunk) == 0)
//			{
//				cout << loop_time << endl;;
//				//每打开一次文件都需要close();否则文件会一直占用。
//				if (laswriter != NULL)
//				{
//					laswriter->close();
//				}
//				stringstream ss;
//				ss << "D:\\outbySzie" << num << ".las";
//				string file_name = ss.str();
//				laswriteopener.set_file_name(file_name.c_str());
//
//				laswriter = laswriteopener.open(&lasreader->header);
//				if (laswriter == 0)
//				{
//					fprintf(stderr, "ERROR: could not open laswriter\n");
//				}
//				num++;
//			}
//			laswriter->write_point(&lasreader->point);
//			laswriter->update_inventory(&lasreader->point);
//			loop_time++;
//		}
//		else {
//			if (loop_time % (count / chunk + 1) == 0)
//			{
//				cout << loop_time << endl;;
//				//每打开一次文件都需要close();否则文件会一直占用。
//				if (laswriter != NULL)
//				{
//					laswriter->close();
//				}
//				stringstream ss;
//				ss << "D:\\outbySzie" << num << ".las";
//				string file_name = ss.str();
//				laswriteopener.set_file_name(file_name.c_str());
//
//				laswriter = laswriteopener.open(&lasreader->header);
//				if (laswriter == 0)
//				{
//					fprintf(stderr, "ERROR: could not open laswriter\n");
//				}
//				num++;
//			}
//			laswriter->write_point(&lasreader->point);
//			laswriter->update_inventory(&lasreader->point);
//			loop_time++;
//		}
//
//	}
//	laswriter->update_header(&lasreader->header, TRUE);
//
//	I64 total_bytes = laswriter->close();
//	delete laswriter;
//	laswriter = nullptr;
//
//	lasreader->close();
//	delete lasreader;
//	lasreader = nullptr;
//}

void lasFileSpiltBySize2(const string &filepath, int blockSize)
{
	LASreadOpener lasreadopener;
	LASwriteOpener laswriteopener;

	blockSize = blockSize * 1024; //MB -> B

	//打开las文件
	lasreadopener.set_file_name(filepath.c_str());
	LASreader* lasreader = lasreadopener.open();
	size_t count = lasreader->header.number_of_point_records;
	printf("%d\n", count);

	int num = 1;
	//LASpoint *one_point = new LASpoint();
	int loop_time = 0;
	LASwriter* laswriter = nullptr;

	int splitnum = (blockSize - sizeof(lasreader->header)) / 28 + 1;
	while (lasreader->read_point())
	{
		if (loop_time %  splitnum == 0)
		{
			cout << loop_time << endl;;
			//每打开一次文件都需要close();否则文件会一直占用。
			if (laswriter != NULL)
			{
				laswriter->close();
			}
			stringstream ss;
			ss << "D:\\outbySzie" << num << ".las";
			string file_name = ss.str();
			laswriteopener.set_file_name(file_name.c_str());

			laswriter = laswriteopener.open(&lasreader->header);
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
//	lasFileSpiltBySize2("D:\\1.las", 500);
//	system("pause");
//}