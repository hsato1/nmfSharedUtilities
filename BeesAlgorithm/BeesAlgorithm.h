#pragma once

#include "Bee.h"
#include "nmfUtils.h"
#include "nmfUtilsStatistics.h"
#include "nmfConstantsMSSPM.h"
#include "nmfGrowthForm.h"
#include "nmfHarvestForm.h"
#include "nmfCompetitionForm.h"
#include "nmfPredationForm.h"

#include "nmfUtilsQt.h"

typedef boost::numeric::ublas::matrix<double> Matrix;

struct beesCompareLT
{
    inline bool operator() (const std::unique_ptr<Bee>& lhsBee,
                            const std::unique_ptr<Bee>& rhsBee)
    {
        return (lhsBee->getFitness() < rhsBee->getFitness());
    }
};

struct beesCompareGT
{
    inline bool operator() (const std::unique_ptr<Bee>& lhsBee,
                            const std::unique_ptr<Bee>& rhsBee)
    {
        return (lhsBee->getFitness() > rhsBee->getFitness());
    }
};

class BeesAlgorithm
{

    const int kTimeToSpendSearching = 30; //3; // time to look for a bee in microseconds

private:
    int                                    m_Seed;
    int                                    m_DefaultFitness;
    int                                    m_NullFitness;
    double                                 m_PatchSizePct;
    std::string                            m_Scaling;
    boost::numeric::ublas::matrix<double>  m_ObsBiomassBySpeciesOrGuilds;
    boost::numeric::ublas::matrix<double>  m_ObsBiomassByGuilds;
    boost::numeric::ublas::matrix<double>  m_Catch;
    boost::numeric::ublas::matrix<double>  m_Effort;
    boost::numeric::ublas::matrix<double>  m_Exploitation;
    std::vector<std::pair<double,double> > m_ParameterRanges;
    std::vector<double>                    m_PatchSizes;
    nmfStructsQt::ModelDataStruct              m_BeeStruct;
    std::unique_ptr<nmfGrowthForm>         m_GrowthForm;
    std::unique_ptr<nmfHarvestForm>        m_HarvestForm;
    std::unique_ptr<nmfCompetitionForm>    m_CompetitionForm;
    std::unique_ptr<nmfPredationForm>      m_PredationForm;
    std::map<int,std::vector<int> >        m_GuildSpecies;
    std::vector<int>                       m_GuildNum;

    std::unique_ptr<Bee> createRandomBee(bool doWhileLoop,
                                         std::string& errorMsg);
    std::unique_ptr<Bee> searchParameterSpaceForBestBee(int& RunNum,
                                                        int& subRunNum,
                                                        std::string& errorMsg);
    void rescaleMinMax(const boost::numeric::ublas::matrix<double> &matrix,
                             boost::numeric::ublas::matrix<double> &rescaledMatrix);
    void rescaleMean(const boost::numeric::ublas::matrix<double> &matrix,
                           boost::numeric::ublas::matrix<double> &rescaledMatrix);
    void rescaleZScore(const boost::numeric::ublas::matrix<double> &matrix,
                             boost::numeric::ublas::matrix<double> &rescaledMatrix);
    std::unique_ptr<Bee> searchNeighborhoodForBestBee(std::unique_ptr<Bee> bestSite,
                                                      int &neighborhoodSize);
    std::unique_ptr<Bee> createNeighborhoodBee(std::vector<double> &bestSiteParameters);
    void printBee(double &fitness, std::vector<double> &parameters);
    void WriteCurrentLoopFile(std::string &MSSPMName,
                              int         &NumGens,
                              double      &BestFitness,
                              int         &NumGensSinceBestFit);
    void printParameterRanges(const int& numSpecies,
                              const int& numGuilds,
                              const std::string& growthForm,
                              const std::string& harvestForm,
                              const std::string& competitionForm,
                              const std::string& predationForm);
    bool StoppedByUser();
    bool isABetterFitness(double& bestFitnessInPopulation,
                          double& bestBeesFitness);
    void loadInitBiomassParameterRanges(
            std::vector<std::pair<double,double> >& parameterRanges,
            nmfStructsQt::ModelDataStruct& dataStruct);

public:
    BeesAlgorithm(nmfStructsQt::ModelDataStruct BeeStruct,
                  const bool &verbose);
   ~BeesAlgorithm();

    void initializeParameterRangesAndPatchSizes(nmfStructsQt::ModelDataStruct& theBeeStruct);
    int calculateActualNumEstParameters();
    bool estimateParameters(double &bestFitness,
                            std::vector<double> &bestParameters,
                            int &RunNum,
                            int &subRunNum,
                            std::string& errorMsg);
    void extractGrowthParameters(const std::vector<double>& parameters,
                                       int&                 startPos,
                                       std::vector<double>& growthRate,
                                       std::vector<double>& carryingCapacity,
                                       double& systemCarryingCapacity);
    void extractHarvestParameters(const std::vector<double>& parameters,
                                  int& startPos,
                                  std::vector<double>& catchabilityRate);
    void extractInitBiomass(const std::vector<double>& parameters,
                            int&                       startPos,
                            std::vector<double>&       initBiomass);
    void extractCompetitionParameters(const std::vector<double>& parameters,
                                      int& startPos,
                                      boost::numeric::ublas::matrix<double>& alpha,
                                      boost::numeric::ublas::matrix<double>& betaSpecies,
                                      boost::numeric::ublas::matrix<double>& betaGuilds,
                                      boost::numeric::ublas::matrix<double>& betaGuildsGuilds);
    void extractPredationParameters(const std::vector<double>& parameters,
                                    int& startPos,
                                    boost::numeric::ublas::matrix<double>& predation);
    void extractHandlingParameters(const std::vector<double>& parameters,
                                   int& startPos,
                                   boost::numeric::ublas::matrix<double>& handling);
    void extractExponentParameters(const std::vector<double>& parameters,
                                   int& startPos,
                                   std::vector<double>& exponents);
    void extractSurveyQParameters(const std::vector<double>& parameters,
                                  int&                       startPos,
                                  std::vector<double>&       surveyQ);
    double evaluateObjectiveFunction(const std::vector<double> &parameters);

};

