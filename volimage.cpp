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
	VolImage::printUILine();
	string baseName = argv[1];

	//read the contents from header file to create vol image object
	string headerFile = "brain_mri_raws/MRI.data";
	fstream reader(headerFile, ios::in);
	int a, b, c; //three parameters in the header file
	reader >> a >> b >> c;
	VolImage v(429, 303, 123);
	v.readImages(baseName);

	//extract input to call correct functions
	if (argc > 2){
		string arg1 = string(argv[2]);
		if (arg1 == "-x"){
			//extract file
			v.extract(atoi(argv[3]), argv[4]);
		} else if (arg1 == "-d"){
			//extract difference map
			v.diffmap(atoi(argv[3]), atoi(argv[4]), argv[5]);
		} else if (arg1 == "-g"){
			//extract front
			v.extractFront(atoi(argv[3]));
		} else {
			cout << "no such flag " << arg1 << endl;
		}
	}
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
	int bytesRequired = 0;

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
				bytesRequired += width;
			}
		} else {
			cout << "Unable to open file: " << fileName << endl;
			VolImage::printUILine();
			exit(0);
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
	cout << "Number of bytes required: " << bytesRequired + sizeof(slices) << endl;
	cout << "Number of images: " << slices.size() << endl; 
	VolImage::printUILine();


	return true;
}

void VolImage::extract(int sliceId, string output_prefix){
	//will extract slice sliceId from the slices and write it to the output file
	string outfileName = "extracted_Images/"+output_prefix + to_string(sliceId) + ".raw";
	fstream writer(outfileName, ios::binary | ios::out | ios::trunc);

	if (writer.is_open()){
		for (int i = 0; i < height; ++i)
		{
			writer.write((char*)slices[sliceId][i], width);
		}
	} else {
		cout << "Unable to create file: " << outfileName << endl;
		return;
	}

	cout << "File extracted in: " << outfileName << endl;
	VolImage::printUILine();
	writer.close();

	//destroy VolImage to release resources avoiding memory leak
	ClearArray();
}

void VolImage::diffmap(int sliceI, int sliceJ, string output_prefix){
	unsigned char** slice = new unsigned char*[height];
	string outfileName = "extracted_Images/" + output_prefix + "_" + to_string(sliceI) + "_" + to_string(sliceJ) + ".raw";
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
		VolImage::printUILine();
		return;
	}


	//close the file streams
	writer.close();

	cout << "Difference between images " << sliceI << " and " << sliceJ << ": " << endl;
	cout << "Difference Map extracted to file: " << output_prefix << endl;
	VolImage::printUILine();

	//destroy VolImage to release resources avoiding memory leak
	ClearArray();
}

void VolImage::extractFront(int sliceI){
	cout << "extracting from front" << endl;
	printUILine();


	string outfileName = "extracted_Images/frontExtraction" + to_string(sliceI) + ".raw";

	fstream writer(outfileName, ios::binary | ios::out | ios::trunc);
	unsigned char** slice = new unsigned char*[numberOfImages];

	if (writer.is_open()){
		for (int i = 0; i < numberOfImages; ++i)
		{
			writer.write((char*)slices[sliceI][i], width);
		}
	} else {
		cout << "Unable to create file: " << outfileName << endl;
		return;
	}

	//destroy VolImage to release resources avoiding memory leak
	for (int i = 0; i < numberOfImages; ++i)
	{
		delete[] slice[i];
	}

	delete[] slice;


}

int VolImage::volImageSize(void){
	return 0;
}

void VolImage::getDimensions(){
	cout << "width: " << this->width << endl;
	cout << "height: " << this->height << endl;
	cout << "numberOfImages: " << this->numberOfImages << endl;
}

void VolImage::printUILine(){
		cout << "----------------------------------------------" << endl;
}

void VolImage::ClearArray(){
	//destroy VolImage to release resources avoiding memory leak
	//Free each sub-array
	for (int j = 0; j < numberOfImages; ++j)
	{
	    for(int i = 0; i < height; ++i) {
	    	//free up the row of pixels in a slice
	        delete[] slices[j][i];   
	    }
	    //free up slice 
	    delete[] slices[j];
	}
}
