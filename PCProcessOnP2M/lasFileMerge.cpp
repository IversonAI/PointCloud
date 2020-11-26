#include<iostream>
#include<lasreader.hpp>
#include<laswriter.hpp>
#include <sstream>
#include <time.h>
#include<string>
#include <vector>

using namespace std;


static double taketime()
{
	return (double)(clock()) / CLOCKS_PER_SEC;
}

void lasFileMerge(vector<string> path, string out)
{
	double start_time = 0;

	LASreadOpener lasreadopener;

 	LASwriteOpener laswriteopener;
	U32 count = 0;
	LASwriter* laswriter = nullptr;
	//
	
	
	for (int i = 0; i < path.size(); i++)
	{
		char *s = (char*)malloc((path[i].length()+1)*sizeof(char));
		path[i].copy(s, path[i].length(), 0);
		lasreadopener.add_file_name(s);
	}
	/*for (int i=0;i<lasreadopener.get_file_name_number();i++)
	{
		cout << lasreadopener.get_file_name(i);
	}*/

	laswriteopener.set_file_name(out.c_str());

	lasreadopener.set_merged(TRUE);
	lasreadopener.set_populate_header(TRUE);
	LASreader* lasreader = lasreadopener.open();

	cout << lasreader->header.number_of_point_records;


	if (lasreader == 0)
	{
		fprintf(stderr, "ERROR: could not open lasreader\n");
	}

	laswriter = laswriteopener.open(&lasreader->header);
	
	if (laswriter == 0)
	{
		fprintf(stderr, "ERROR: could not open laswriter\n");
	}

	while (lasreader->read_point())
	{
		laswriter->write_point(&lasreader->point);
	}

	// close the writer
	laswriter->update_header(&lasreader->header, TRUE);
	
	laswriter->close();
	fprintf(stderr, "merging files took %g sec.\n", taketime() - start_time);
	delete laswriter;

	lasreader->close();

	delete lasreader;

}

int main()
{	
	vector<string> inputpath = { "D:\\out1.las","D:\\out2.las","D:\\out3.las" };
	string outputpath = "D:\\merged.las";
	lasFileMerge(inputpath, outputpath);
	system("pause");
	return 0;
}
