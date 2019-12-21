//Author::Cand --
//Time::2019-10-15
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <windows.h>
// 点云结构体
typedef struct point
{
    double x;
    double y;
    double z;
    double intensity;
}PointXYZT;

using namespace std;

//To Get the PointCloud Max and min Value
double
GetMinMaxValue(const string str,PointXYZT* min, PointXYZT* max);
//To Calculate the distance between two point
double
CalDistance(PointXYZT a,PointXYZT b);
//To Slice the Point b Value (b,0,0,1.2)
double
SlicePoint(string str, PointXYZT b);
//To Get a Cylinder by Radius
double
CutCyliner(const string str,const PointXYZT CenterPos,const double Radius);
//Filter Point Cloud By Z Vlaue;
int
FilterByZValue(const string str,const PointXYZT min,double disThreshold);
//
int
TestPara(const PointXYZT PointPa1,const PointXYZT PointPa2);
//
int
Value(const string str_SourceFile);

//
int Pcd2Txt(const string str_SourceFile)
{
    ifstream fileIn(str_SourceFile.c_str());

    if (fileIn == NULL)
    {
        cout <<"Open file Error(txt2pcd)\n";
        return 1;
    }

    string str_saveFile="Test.txt";
    ofstream fileout(str_saveFile.c_str());

    stringstream ss;
    string buffer;
    PointXYZT pt1;
    int i=0;
    while(getline(fileIn,buffer))
    {
        if( i<11)
        {
            i++;
            continue;
        }
        ss.clear();
        ss.str(buffer);
        fileout << buffer << endl;
    }

    fileIn.close();
    fileout.close();

    return 0;
}

//
int main()
{
    //read the initial file
    ifstream fConfig("..//CircleCutPoint//Config//Circut_config");
    if (!fConfig)
    {
        cout << "Open Config file Error\n";
        exit(1);
    }
    else
    {
        cout << "Read Config file Success\n";
    }
    string str_SourceFile;
    double radius;
    string str_SaveFile2;

    //Read the Config file and close file
    fConfig >> str_SourceFile >> str_SaveFile2 >>radius;
    fConfig.close();

    //Pcd2Txt(str_SourceFile);

    if (radius <= 0)
    {
        cout << "the Radius in wrong Value!!\n";
    }


    PointXYZT p1;
    p1.x = 0;
    p1.y = 0;
    p1.z = 1.2;

    //获得最大值和最小值
    PointXYZT MaxValue;
    PointXYZT MinValue;
    GetMinMaxValue(str_SourceFile,&MinValue,&MaxValue);


    cout << "The Max::" << MaxValue.x << "   " << MaxValue.y << "  "<< MaxValue.z << endl;
    cout << "The Min::" << MinValue.x << "   " << MinValue.y << "  "<< MinValue.z << endl;

    //FilterByZValue(str_SourceFile,MinValue,5.0);


    PointXYZT pt1;
    pt1.x = MinValue.x;
    pt1.y = MinValue.y;
    pt1.z = MinValue.z;
    //CutCyliner(str_SourceFile,pt1,1.0);
    //TestPara(pt1,MaxValue);



    SlicePoint(str_SourceFile,pt1);

    //PointXYZT Center;
    //Center.x = 0;
    //Center.y = 0;
    //Center.z = 0;
    //CutCyliner(str_SourceFile,Center,10.0);


    cout << "The Program has over!!!";


    return 0;
}

//Calculate the distance between two point
double CalDistance(PointXYZT a,PointXYZT b)
{
    double dis = 0.0;

    dis = (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z);

    return sqrt(dis);
}

//Slice the Point
double SlicePoint(string str, PointXYZT b)
{
    ifstream fileIn(str.c_str());

    if (fileIn == NULL)
    {
        printf("Open the file root fail\n");
        exit(1);
    }
    const double height = 2.0;
    const double DBHheight = 1.3;
    string str_SaveFile;
    str_SaveFile = "newSlice.txt";
    ofstream fileOut(str_SaveFile.c_str());

    stringstream ss;
    string buffer;
    PointXYZT point1;
    double treeHeight = 0.0;
    //Calculate the requirement
    while(getline(fileIn,buffer))
    {
        ss.clear();
        ss.str(buffer);
        ss >> point1.x >> point1.y >> point1.z;

        treeHeight = sqrt((point1.z-b.z)*(point1.z-b.z));
        if (treeHeight < height+0.01 && treeHeight > height-0.01)
        {
            fileOut << buffer << endl;
        }
    }

    fileIn.close();
    fileOut.close();

    return 1;
}

//Cut the Point Cloud use the Cylinder
double CutCyliner(const string str,const PointXYZT CenterPos,const double Radius)
{
    ifstream fileIn(str.c_str());
    if (fileIn == NULL)
    {
        printf("Open the File root file\n");
        exit(1);
    }

    PointXYZT point1;
    PointXYZT sourPoint;
    sourPoint.x = CenterPos.x;
    sourPoint.y = CenterPos.y;
    sourPoint.z = CenterPos.z;

    double radius=Radius;


    //cout << "B-- Point::" << b.x << "  " << b.y << "  " << b.z << endl;
    cout << "Source Point::" << sourPoint.x << "  " << sourPoint.y << "  " << sourPoint.z << endl;

    cout << "Radius::" << Radius;
    string str_SaveFileName = str + "Radius" +".txt";
    ofstream fileOut(str_SaveFileName.c_str());

    stringstream ss;
    string buffer;

    //Calculate the requirement
    while(getline(fileIn,buffer))
    {
        ss.clear();
        ss.str(buffer);
        ss >> point1.x >> point1.y >> point1.z;
        point1.z = 0;
        if (CalDistance(point1,sourPoint)<=radius)
        {
            fileOut << buffer << endl;
        }
    }

    //Close the file
    fileIn.close();
    fileOut.close();

    return 1;
}


double GetMinMaxValue(const string str, PointXYZT* min, PointXYZT* max)
{
    ifstream fin(str.c_str());

    if (fin == NULL)
    {
        cout << "the File Open Error(GetMinMax moudle)\n";
        exit(1);
    }
    max->z = -10000.0;

    min->z = 100000.0;

    PointXYZT p1;
    stringstream ss;
    string buffer;
    PointXYZT px;

    while(getline(fin,buffer))
    {
        ss.clear();
        ss.str(buffer);
        ss >> p1.x >> p1.y >> p1.z;

        if (max->z <= p1.z)
        {
            max->x = p1.x;
            max->y = p1.y;
            max->z = p1.z;
        }

        if (  p1.z <= min->z)
        {
            min->x = p1.x;
            min->y = p1.y;
            min->z = p1.z;
        }
    }

    return 1;
}
//按照距离小于disThreshold的滤除
int FilterByZValue(const string str,const PointXYZT min, double disThreshold)
{
    ifstream fin(str.c_str());

    if (fin ==NULL)
    {
        std::cout << "File Open Error(Function::FilterByZValue)" << std::endl;
        return 1;
    }

    string saveFileName = str + "Filter" + ".txt";
    ofstream out(saveFileName.c_str());

    PointXYZT Apoint;

    PointXYZT minPoint;
    minPoint.x = 0;
    minPoint.y = 0;
    minPoint.z = min.z;

    stringstream ss;
    string buffer;
    double dis;

    PointXYZT Bpoint;
    Bpoint.x = 0;
    Bpoint.y = 0;


    while(getline(fin,buffer))
    {
        ss.clear();
        ss.str(buffer);
        ss >> Apoint.x >> Apoint.y >> Apoint.z;
        Bpoint.z = Apoint.z;
        dis = CalDistance(Bpoint,minPoint);
        if(dis <= disThreshold)
        {
            out << buffer << endl;
        }
    }

    fin.close();
    out.close();
}


int Value(const string str_SourceFile)
{
    ifstream fileIn(str_SourceFile.c_str());

    if (fileIn == NULL)
    {
        cout << "ERROR \n";
        return 0;
    }


    string str_SaveFile;
    str_SaveFile = "newCutZ.txt";
    ofstream fileOut(str_SaveFile.c_str());

    stringstream ss;
    string buffer;
    PointXYZT MinValue;
    PointXYZT point1;
    double dis = 0.0;
    MinValue.x = 0.0;
    MinValue.y = 0.0;
    //Calculate the requirement
    while(getline(fileIn,buffer))
    {
        ss.clear();
        ss.str(buffer);
        ss >> point1.x >> point1.y >> point1.z;
        point1.x = 0;
        point1.y = 0;
        dis = CalDistance(point1,MinValue);
        if (dis > 1.0)
        {
            fileOut << buffer << endl;
        }
    }

    fileIn.close();
    fileOut.close();
}

int TestPara(const PointXYZT Pa1,const PointXYZT Pa2)
{
    PointXYZT a1;
    PointXYZT a2;

    a1.x = Pa1.x;
    a1.y = Pa1.y;
    a1.z = Pa1.z;

    cout << "Pa1::" << Pa1.x << " " << Pa1.y << "  "<< Pa1.z << endl;
    cout << "a1::" << a1.x << " " << a1.y << "  "<< a1.z << endl;

    return 0;

}
