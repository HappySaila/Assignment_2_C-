//WLSGRA012
#include <iostream>
#include <vector>
#include <fstream>


#include "Volimage.h"
#include "cstdlib"
#include <cmath>

using namespace std;
using namespace WLSGRA012;

//prototyping
void Debug(string s);

//variables

int main(int argc, char const *argv[])
{
	string baseName = argv[1];
	//read files
	VolImage v(429, 303,123);
	v.readImages(baseName);

	//extract input to call correct functions
	if (argc > 2){
		string arg1 = string(argv[2]);
		if (argc > 2 && argc <  7){
			//only one argument was parsed
			if (arg1 == "-x"){
				//call extract
				v.extract(atoi(argv[3]), argv[4]);
			} else if (arg1 == "-d"){
				v.diffmap(atoi(argv[3]), atoi(argv[4]), argv[5]);
			} else {
				cout << "no such flag " << arg1 << endl;
			}
		} else if (argc > 6){
			//two arguments were parsed
			if (arg1 == "-x"){
				//call extract
				v.extract(atoi(argv[3]), argv[4]);
				v.diffmap(atoi(argv[6]), atoi(argv[7]), argv[8]);
			} else if (arg1 == "-d"){
				v.diffmap(atoi(argv[3]), atoi(argv[4]), argv[5]);
				v.extract(atoi(argv[7]), argv[8]);
			} 
		}
	}



	//extract files

	//extract difference map
	return 0;
}

//VolImage implementations
VolImage::VolImage(int width, int height, int numberOfImages){
	this->width = width;
	this->height = height;
	this->numberOfImages = numberOfImages;
}

VolImage::~VolImage(){
	width = 0;
	height = 0;
}

bool VolImage::readImages(string baseName){
	//extract all images based on the based name
	//eg will extract images scan1 ...scanN will base name scan
	//all images will be saved into the objects slices vector

	for (int i = 0; i < numberOfImages; i++)
	{
		//extract the ith image
		//get the file name
		string fileName = "brain_mri_raws/" + baseName + to_string(i) + ".raw";
		//open the file using the fstream
		//parse in write and read fuctionality using ios::in and ios::out
		//parse is binary attrib so that the reader knows to read binary objects
		fstream reader(fileName, ios::binary | ios::in);
		//create slice
		unsigned char** slice = new unsigned char*[height];

		if (reader.is_open()){
			for (int j = 0; j < height; j++)
			{
				//get each row. therefore the loop will terminate after it has iterated through the height of the image
				slice[j] = new unsigned char[width];
				reader.read((char*)slice[j], width);
			}
		} else {
			cout << "Unable to open file: " << fileName << endl;
			return false;
		}

		//push the slice into the vector of the volImage object
		slices.push_back(slice);

		//close the reader as we have read in all the data
    	reader.close();

	}
	//all images have been extracted

	//print number of images and number of bytes required
	//TODO: calculate the number of bytes required
	cout << "Number of bytes required: " << 56 << endl;
	cout << "Number of images: " << slices.size() << endl; 

	return true;
}

void VolImage::extract(int sliceId, string output_prefix){
	//will extract slice sliceId from the slices and write it to the output file
	string outfileName = "extracted_Images/"+output_prefix + to_string(sliceId) + ".raw";
	fstream reader(outfileName, ios::binary | ios::out | ios::trunc);

	if (reader.is_open()){
		for (int i = 0; i < height; ++i)
		{
			reader.write((char*)slices[sliceId][i], width);
		}
	} else {
		cout << "Unable to create file: " << outfileName << endl;
		return;
	}

	cout << "File created in: " << outfileName << endl;
	reader.close();
}

void VolImage::diffmap(int sliceI, int sliceJ, string output_prefix){
	unsigned char** slice = new unsigned char*[height];
	string outfileName = "DiffMap_Images/" + output_prefix + "_" + to_string(sliceI) + "_" + to_string(sliceJ) + ".raw";
	fstream writer(outfileName, ios::out | ios::trunc | ios::binary);

	if (writer.is_open()){
		for (int i = 0; i < height; ++i)
		{
			slice[i] = new unsigned char[width];	
			for (int j = 0; j < width; ++j)
			{
				slice[i][j]  = (unsigned char)(abs((float)slices[sliceI][i][j] - (float)slices[sliceJ][i][j])/2);
			}
			writer.write((char*)slice[i], width);
		}
	} else {
		cout << "Unable to write to file: " << outfileName << endl;
		return;
	}


	//close the file streams
	writer.close();

	cout << "difference between: " << sliceI << " and " << sliceJ << endl;
	cout << "to output file: " << output_prefix << endl;
}

int VolImage::volImageSize(void){
	return 0;
}

void VolImage::getDimensions(){
	cout << "width: " << this->width << endl;
	cout << "height: " << this->height << endl;
	cout << "numberOfImages: " << this->numberOfImages << endl;
}
