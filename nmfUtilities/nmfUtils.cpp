
/** @file nmfUtils.h
 * @brief Definition for common typedefs and structs (i.e., non-graphical)
 * @date Dec 8, 2016
 *
 * This file contains typedefs, structs, and functions common to the MultiSpecies
 * applications. The functions that print various structures are located in this
 * file as well.
 */

#include <ctime>
#include <fstream>
#include "nmfUtils.h"
#include "nmfConstants.h"
#include "nmfLogger.h"

namespace nmfUtils {

void printError(const std::string msg, const std::string errorMsg)
{
    std::cout << "\nError: " << errorMsg << std::endl;
    std::cout << msg << std::endl;
}

void printMsg(std::string msg) {
    std::cout << msg << std::endl;
}

void printMapPair(const std::pair<std::string, std::vector<std::string> > &pair) {
    std::cout << "Key: " << pair.first << std::endl;
    copy(pair.second.begin(), pair.second.end(),
            std::ostream_iterator<std::string>(std::cout, "\n"));
}

void printMap(const std::string name, const std::map<std::string, std::vector<std::string> > &dataMap) {
    std::cout << "Map: " << name << std::endl;
    std::for_each(dataMap.begin(), dataMap.end(), printMapPair);
}

void printMatrix(const std::string &name,
        const boost::numeric::ublas::matrix<double> &mat,
        const int width,
        const int precision) {
    std::cout << "\n" << name << " (" << mat.size1() << "x" << mat.size2()
            << ")" << std::endl;
    for (unsigned i = 0; i < mat.size1(); ++i) {
        std::cout << "  ";
        for (unsigned j = 0; j < mat.size2(); ++j) {
            std::cout << std::setw(width)
                      << std::setprecision(precision)
                      << std::fixed << mat(i, j) << "  ";
        }
        std::cout << std::endl;
    }
}


double getMatrixSum(const boost::numeric::ublas::matrix<double> &mat) {
    return sum(prod(boost::numeric::ublas::scalar_vector<double>(mat.size1()), mat));
}

double getVectorSum(const boost::numeric::ublas::vector<double> &vec) {
    return sum(vec);
}



void print3DArray(
        const std::string &name,
        const std::vector<int> &dimensions,
        const Boost3DArrayDouble &array,
        const int &width,
        const int &precision)
{
    int imax = dimensions[0];
    int jmax = dimensions[1];
    int kmax = dimensions[2];

    std::cout <<  name.c_str() << " (" << imax << "x" << jmax << "x" << kmax << "):\n" << std::endl;
    for (int i = 0; i < imax; ++i) {
        for (int j = 0; j < jmax; ++j) {
            for (int k = 0; k < kmax; ++k) {
                std::cout << std::fixed << std::setw(width) << std::setprecision(precision) << array[i][j][k] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void print4DArray(
        const std::string &name,
        const std::vector<int> &dimensions,
        const Boost4DArrayDouble &array)
{
    int imax = dimensions[0];
    int jmax = dimensions[1];
    int kmax = dimensions[2];
    int lmax = dimensions[3];

    std::cout << name.c_str() << " (" << imax << "x" << jmax << "x" << kmax << "x" << lmax
            << "):\n" << std::endl;
    for (int i = 0; i < imax; ++i) {
        for (int j = 0; j < jmax; ++j) {
            for (int k = 0; k < kmax; ++k) {
                for (int l = 0; l < lmax; ++l) {
                    std::cout << array[i][j][k][l] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}


// RSK - improve with templates...

void printVector(const std::string &name,
        const boost::numeric::ublas::vector<std::string> &vec) {
    std::cout << "\n" << name << ": " << vec.size() << std::endl;
    for (unsigned i = 0; i < vec.size(); ++i) {
        std::cout << "| ";
        std::cout << std::setw(30) << vec(i) << " | ";
        std::cout << std::endl;
    }
}

void printVector(const std::string &name,
                 const std::vector<std::string> &vec)
{
    std::cout << "\n" << name << ": " << vec.size() << std::endl;
    for (unsigned i = 0; i < vec.size(); ++i) {
        std::cout << "| ";
        std::cout << std::setw(4) << vec[i] << " | ";
        std::cout << std::endl;
    }
}

void printVector(const std::string &name,
        const boost::numeric::ublas::vector<double> &vec) {
    std::cout << "\n" << name << ": " << vec.size() << std::endl;
    for (unsigned i = 0; i < vec.size(); ++i) {
        std::cout << "| ";
        std::cout << std::setw(4) << vec(i) << " | ";
        std::cout << std::endl;
    }
}

void initialize(std::vector<double>& vec, const int& numValues)
{
    for (int i=0; i<numValues; ++i) {
        vec.push_back(0.0);
    }
}

void initialize(boost::numeric::ublas::vector<std::string> &vec, const int &nrows) {
    vec.resize(nrows);
    vec.clear();
}
void initialize(boost::numeric::ublas::vector<int> &vec, const int &nrows) {
    vec.resize(nrows);
    vec.clear();
}
void initialize(boost::numeric::ublas::vector<double> &vec, const int &nrows) {
    vec.resize(nrows);
    vec.clear();
}
void initialize(boost::numeric::ublas::matrix<int> &mat,
                const int &nrows,
                const int &ncols)
{
    mat.resize(nrows,ncols,false);
    mat.clear();
}
void initialize(boost::numeric::ublas::matrix<double> &mat,
                const int &nrows,
                const int &ncols)
{
    mat.resize(nrows,ncols,false);
    mat.clear();
}
void initialize(Boost3DArrayDouble &array3d)
{
    for (unsigned int i=0; i<array3d.shape()[0]; ++i)
        for (unsigned int j=0; j<array3d.shape()[1]; ++j)
            for (unsigned int k=0; k<array3d.shape()[2]; ++k)
                array3d[i][j][k] = 0.0;
}
void initialize(Boost4DArrayDouble &array4d)
{
    for (unsigned int i=0; i<array4d.shape()[0]; ++i)
        for (unsigned int j=0; j<array4d.shape()[1]; ++j)
            for (unsigned int k=0; k<array4d.shape()[2]; ++k)
                for (unsigned int l=0; l<array4d.shape()[3]; ++l)
                    array4d[i][j][k][l] = 0.0;
}
void initialize(Boost5DArrayDouble &array5d)
{
    for (unsigned int i=0; i<array5d.shape()[0]; ++i)
        for (unsigned int j=0; j<array5d.shape()[1]; ++j)
            for (unsigned int k=0; k<array5d.shape()[2]; ++k)
                for (unsigned int l=0; l<array5d.shape()[3]; ++l)
                    for (unsigned int m=0; m<array5d.shape()[4]; ++m)
                        array5d[i][j][k][l][m] = 0.0;
}

bool invertMatrix(
        boost::numeric::ublas::matrix<double>& matrix,
        boost::numeric::ublas::matrix<double>& inverseMatrix)
{
    bool retv = true;

    initialize(inverseMatrix,int(matrix.size1()),int(matrix.size2()));

    try {
        inverseMatrix = boost::numeric::ublas::identity_matrix<float>(matrix.size1());
        boost::numeric::ublas::permutation_matrix<std::size_t> pm(matrix.size1());
        boost::numeric::ublas::lu_factorize(matrix,pm);
        boost::numeric::ublas::lu_substitute(matrix, pm, inverseMatrix);
    }  catch (...) {
        retv = false;
    }

    return retv;
}

double getMatrixMax(boost::numeric::ublas::matrix<double> &mat,
                         bool roundOff=false)
{
    // find largest value in mat
    double maxVal = 0.0;
    double val = 0.0;
    for (boost::numeric::ublas::matrix<double>::iterator1 it1 =
            mat.begin1(); it1 != mat.end1(); ++it1) {
        for (boost::numeric::ublas::matrix<double>::iterator2 it2 = it1.begin();
                it2 != it1.end(); ++it2) {
            val = *it2;
            if (val > maxVal) {
                maxVal = val;
            }
        }
    }
    if (maxVal > 1.0) {
        if (roundOff) {
            int numDigits = int(log10(maxVal) + 1);
            int firstDigit = int((maxVal / pow(10.0, numDigits - 1)) + 0.5);
            maxVal = firstDigit * pow(10.0, numDigits - 1);
        }
    } else {
        maxVal = 1.0;
    }
    return maxVal;

}

/*
 * Returns a random number between the passed limits: [lowerLimit,upperLimit)
 * If seed < 0, no seed is used and the algorithm is stochastic.
 * If seed >= 0, then that seed is used and the algorithm is deterministic.
 */
double getRandomNumber(int seedValue, double lowerLimit, double upperLimit)
{
//    std::random_device rd;
//    int RandomSeed = (seed < 0) ? rd() : seed;
//    std::mt19937 e2(RandomSeed);
//    std::uniform_real_distribution<> dist(lowerLimit,upperLimit);
//    return dist(e2);

    unsigned RandomSeed = nmfUtilsQt::getCurrentTime().currentMSecsSinceEpoch();
    unsigned Seed = (seedValue < 0) ? RandomSeed : seedValue;
    std::uniform_real_distribution<double> dist(lowerLimit,upperLimit);
    std::mt19937_64 rng(Seed);
    return dist(rng);
}

bool isNearlyZero(const double& value)
{
    return (std::abs(value) < nmfConstants::NearlyZero);
}

bool isWholeNumber(double value)
{
    return (floor(value) == ceil(value));
}

double round(double number, int decimalPlaces)
{
    if (decimalPlaces < 0) {
        std::cout << "Error: decimalPlaces argument in nmfUtils::round() function must be >= 0." << std::endl;
        decimalPlaces = 0;
    }

    double factor = std::pow(10,decimalPlaces);
    return  std::floor(number*factor+0.5)/factor;
}

void split(std::string main, std::string delim, std::string &str1, std::string &str2)
{
    if (main.empty()) {
        return;
    }
    std::string main2 = main;
    size_t sep = main2.find(delim);
    if (sep == 0) {
        return;
    }

    str1 = main2.erase(sep,main2.size());
    main2 = main;
    str2 = main2.erase(0,sep+1);
}

double convertCatchUnitsToValue(std::string catchUnits)
{
    double CatchUnitsVal = 0.0;

    if (catchUnits == "Hundreds of Fish")
        CatchUnitsVal =    0.1;
    else if (catchUnits == "Thousands of Fish")
        CatchUnitsVal =    1.0;
    else if (catchUnits == "Millions of Fish")
        CatchUnitsVal = 1000.0;

    return CatchUnitsVal;
}

double convertWeightUnitsToValue(std::string weightUnits)
{
    double WeightUnitsVal = 0.0;

    if (weightUnits == "Gram")
        WeightUnitsVal = 0.001;
    else if (weightUnits == "Kilograms")
        WeightUnitsVal = 1.0;
    else if (weightUnits == "Pounds")
        WeightUnitsVal = (1.0/2.2);

    return WeightUnitsVal;
}

double convertSizeUnitsToValue(std::string sizeUnits)
{
    double SizeUnitsVal = 0.0;

    if (sizeUnits == "Millimeters")
        SizeUnitsVal = 0.1;
    else if (sizeUnits == "Centimeters")
        SizeUnitsVal = 1.0;
    else if (sizeUnits == "Inches")
        SizeUnitsVal = 2.54;

    return SizeUnitsVal;
}

void readTableDescriptions(std::string TableName,
                          std::map<std::string,std::string> &TableDescription)
{
    std::string msg;
    std::string line;
    std::vector<std::string> parts;

    TableDescription.clear();

    std::ifstream fptr(TableName);
    if (fptr) {
        while (std::getline(fptr,line))
        {
            boost::algorithm::trim(line); // trims any spaces
            if (line[0] != '#') {
                boost::split(parts,line,boost::is_any_of("-"),boost::token_compress_on);
                if (parts.size() == 2) {
                    boost::algorithm::trim(parts[0]);
                    boost::algorithm::trim(parts[1]);
                    TableDescription[parts[0]] = parts[1];
                }
            }
        }
        fptr.close();
    } else {
        msg = "\nnmfUtils::readTableDescription Error: Couldn't open file: " + TableName;
        //QMessageBox::warning(0,tr("File Open Error"),tr(msg.c_str()),QMessageBox::Ok);
        std::cout << msg << std::endl;
    }
} // end readTableDescription


void readTableNames(std::map<std::string,std::vector<std::string> > *TableNames)
{
    int modelType;
    std::string file = nmfConstantsMSVPA::TableNamesFile;
    std::string line;
    std::string msg;
    std::vector<std::string> parts;
    std::ifstream fptr(file);
    std::vector<std::string> fields;

    for (auto i=0;i<nmfConstantsMSVPA::NumModelTypes; ++i) {
        TableNames[i].clear();
    }
    if (fptr) {
        while (std::getline(fptr,line))
        {
            boost::algorithm::trim(line); // trims any spaces
            if (line[0] != '#') {
                boost::split(parts,line,boost::is_any_of("\t "),boost::token_compress_on);
                if (parts.size() > 1) {
                    fields.clear();
                    for (int i=0; i<std::stoi(parts[2]); ++i) {
                        fields.push_back(parts[i+3]);
                    }
                    modelType = (parts[1] == "SSVPA") ? 0 : (parts[1] == "MSVPA") ? 1 : 2 ;
                    TableNames[modelType][parts[0]] = fields;
                } else {
                    std::cout << "nmfUtils::readTableNames Error in .MSVPA_X2/data/tableNames.dat." << std::endl;
                    break;
                }
            }
        }
        fptr.close();
    } else {
        msg = "\nnmfUtils::readTableNames Error: Couldn't open file: " + file;
        //QMessageBox::warning(0,tr("File Open Error"),tr(msg.c_str()),QMessageBox::Ok);
        std::cout << msg << std::endl;
    }
} // end readTableNames



bool checkForError(nmfLogger *logger,
                   std::string table,
                   std::string field,
                   std::map<std::string, std::vector<std::string> > &dataMap,
                   std::string queryStr)
{
    if (dataMap[field].size() == 0) {
        logger->logMsg(nmfConstants::Error,"Error - Check " + table + " table. ");
        logger->logMsg(nmfConstants::Error,"Error - No data found for cmd: "+queryStr);
        return true; // there was an error
    }

    return false; // there was no error

} // end errorCheck


/*
 * Rescales matrix Xij by: log base 10 (X)
 */
void
rescaleMatrixLog(boost::numeric::ublas::matrix<double> &Matrix)
{
    int NumRows = Matrix.size1();
    int NumCols = Matrix.size2();

    // Rescale the matrix using the min and max values found for each species
    for (auto species=0; species<NumCols; ++species) {
        for (auto time=0; time<NumRows; ++time) {
            Matrix(time,species) = std::log10(Matrix(time,species));
        }
    }
}

/*
 * Rescales matrix Xij by: (X - Xmin) / (Xmax - Xmin)
 */
void
rescaleMatrixMinMax(boost::numeric::ublas::matrix<double> &Matrix)
{
    double den;
    double min;
    double max;
    int NumRows = Matrix.size1();
    int NumCols = Matrix.size2();
    std::vector<double> MinValues;
    std::vector<double> MaxValues;

    // Find min and max values for each species
    for (auto species=0; species<NumCols; ++species) {
        boost::numeric::ublas::matrix_column<boost::numeric::ublas::matrix<double> > aMatrixColumn(Matrix,species);
        auto result = std::minmax_element(aMatrixColumn.begin(), aMatrixColumn.end());
        MinValues.push_back(*result.first);
        MaxValues.push_back(*result.second);
    }

    // Rescale the matrix using the min and max values found for each species
    for (auto species=0; species<NumCols; ++species) {
        min = MinValues[species];
        max = MaxValues[species];
        den = (max - min);
        for (auto time=0; time<NumRows; ++time) {
            Matrix(time,species) = (Matrix(time,species) - min) / den;
        }
    }
}

void reset(boost::numeric::ublas::matrix<double>& mat,
           const double& resetValue)
{
    for (int i=0; i<int(mat.size1()); ++i) {
        for (int j=0; j<int(mat.size2()); ++j) {
            mat(i,j) = resetValue;
        }
    }
}

bool isEstimateParameterChecked(
        const nmfStructsQt::ModelDataStruct& dataStruct,
        const std::string& ParameterName)
{
    bool isChecked = false;

    std::vector<nmfStructsQt::EstimateRunBox> EstimateRunBoxes = dataStruct.EstimateRunBoxes;
    for (nmfStructsQt::EstimateRunBox runBox : EstimateRunBoxes) {
        if (runBox.parameter == ParameterName) {
            isChecked = true;
            break;
        }
    }

    return isChecked;
}

std::string
convertToScientificNotation(double val)
{
    std::ostringstream streamDouble; // Needed to stuff a double into a string with scientific notation

    streamDouble.str("");
    streamDouble << val;

    return streamDouble.str();
}

void
getFilesWithExt(std::string path,
                std::string ext,
                QStringList& filesToCopy)
{
    QString fullExt = "*" + QString::fromStdString(ext);
    filesToCopy.clear();

    QDir directory(QString::fromStdString(path));
    filesToCopy = directory.entryList(QStringList() << fullExt << fullExt,QDir::Files);
}

void
copyFile(std::string fileA,
         std::string fileB)
{
    std::ifstream fileIn( fileA, std::ios::in);
    std::ofstream fileOut(fileB, std::ios::out);

    fileOut << fileIn.rdbuf();

    fileIn.close();
    fileOut.close();
}

bool
isOSWindows()
{
    std::string osName = getOS();

    return ((osName == "Windows 32-bit") ||
            (osName == "Windows 64-bit"));
}

std::string
getOS()
{
    // The _WIN64 check should be first, since _WIN32 is 1 for both 32-bit and 64-bit
    #ifdef _WIN64
        return "Windows 64-bit";
    #elif _WIN32
        return "Windows 32-bit";
    #elif __APPLE__ || __MACH__
        return "Mac OSX";
    #elif __linux__
        return "Linux";
    #elif __FreeBSD__
        return "FreeBSD";
    #elif __unix || __unix__
        return "Unix";
    #else
        return "Other";
    #endif
}

void
append(std::vector<double>& newVec,
       std::vector<double>& currentVec)
{
    currentVec.insert(currentVec.end(), newVec.begin(), newVec.end());
}

}
