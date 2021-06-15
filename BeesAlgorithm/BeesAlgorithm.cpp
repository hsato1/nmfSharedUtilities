#include "BeesAlgorithm.h"



BeesAlgorithm::BeesAlgorithm(nmfStructsQt::ModelDataStruct theBeeStruct,
                             const bool &verbose)
{
    bool isAggProd  = (theBeeStruct.CompetitionForm == "AGG-PROD");
    int  numSpecies =  theBeeStruct.NumSpecies;
    int  numGuilds  =  theBeeStruct.NumGuilds;
    m_BeeStruct      = theBeeStruct;
    m_Seed           = -1;
    m_DefaultFitness =  99999;
    m_NullFitness    = -999.9;

    std::string growthForm      = theBeeStruct.GrowthForm;
    std::string harvestForm     = theBeeStruct.HarvestForm;
    std::string competitionForm = theBeeStruct.CompetitionForm;
    std::string predationForm   = theBeeStruct.PredationForm;

    m_GuildSpecies = theBeeStruct.GuildSpecies;
    m_GuildNum     = theBeeStruct.GuildNum;

    // Get number of independent runs
    m_Scaling = theBeeStruct.ScalingAlgorithm;
    if (verbose) {
        std::cout << "\nBeesAlgorithm: Parameters to Estimate: " << m_BeeStruct.TotalNumberParameters << std::endl;
    }
    // Make patch size a percentage of parameter space
    m_PatchSizePct = m_BeeStruct.BeesNeighborhoodSize/100.0;
    if (verbose) {
        std::cout << "BeesAlgorithm: Patch Size Pct: " << m_PatchSizePct << std::endl;
        std::cout << "BeesAlgorithm: Scaling Algorithm: " << m_Scaling << std::endl;
    }

    // Define growth, harvest, competition, and predation forms
    m_GrowthForm      = std::make_unique<nmfGrowthForm>(growthForm);
    m_HarvestForm     = std::make_unique<nmfHarvestForm>(harvestForm);
    m_CompetitionForm = std::make_unique<nmfCompetitionForm>(competitionForm);
    m_PredationForm   = std::make_unique<nmfPredationForm>(predationForm);

    // Set up default parameters ranges and neighborhood patch sizes
    initializeParameterRangesAndPatchSizes(theBeeStruct);

    if (verbose) {
        std::cout << "BeesAlgorithm: Initialized " << m_ParameterRanges.size() << " parameter ranges" << std::endl;
    }

    // Echo parameter ranges to shell
    if (verbose) {
        printParameterRanges(numSpecies,numGuilds,growthForm,harvestForm,
                             competitionForm,predationForm);
    }

    // Get the observed biomass
    if (isAggProd) {
        nmfUtils::initialize(m_ObsBiomassBySpeciesOrGuilds,
                             theBeeStruct.ObservedBiomassByGuilds.size1(),
                             theBeeStruct.ObservedBiomassByGuilds.size2());
        m_ObsBiomassBySpeciesOrGuilds = theBeeStruct.ObservedBiomassByGuilds;

    } else {
        nmfUtils::initialize(m_ObsBiomassBySpeciesOrGuilds,
                             theBeeStruct.ObservedBiomassBySpecies.size1(),
                             theBeeStruct.ObservedBiomassBySpecies.size2());
        m_ObsBiomassBySpeciesOrGuilds = theBeeStruct.ObservedBiomassBySpecies;
    }

    nmfUtils::initialize(m_ObsBiomassByGuilds,
                         theBeeStruct.ObservedBiomassByGuilds.size1(),
                         theBeeStruct.ObservedBiomassByGuilds.size2());
    m_ObsBiomassByGuilds = theBeeStruct.ObservedBiomassByGuilds;
//nmfUtils::printMatrix("BiomassByGuild",m_ObsBiomassByGuilds,10,6);
    if (verbose) {
        std::cout << "BeesAlgorithm: Read Observed Biomass" << std::endl;
    }

    // Get the Catch
    nmfUtils::initialize(m_Catch,
                         theBeeStruct.Catch.size1(),
                         theBeeStruct.Catch.size2());
    m_Catch = theBeeStruct.Catch;
    if (verbose) {
        std::cout << "BeesAlgorithm: Read Catch" << std::endl;
    }

    // Get the Effort
    nmfUtils::initialize(m_Effort,
                         theBeeStruct.Effort.size1(),
                         theBeeStruct.Effort.size2());
    m_Effort = theBeeStruct.Effort;
    if (verbose) {
        std::cout << "BeesAlgorithm: Read Effort" << std::endl;
    }

    // Get the Exploitation
    nmfUtils::initialize(m_Exploitation,
                         theBeeStruct.Exploitation.size1(),
                         theBeeStruct.Exploitation.size2());
    m_Exploitation = theBeeStruct.Exploitation;
    if (verbose) {
        std::cout << "BeesAlgorithm: Read Exploitation" << std::endl;
    }
std::cout << "BeesAlgorithm::BeesAlgorithm end" << std::endl;
}

BeesAlgorithm::~BeesAlgorithm()
{
}


void
BeesAlgorithm::printParameterRanges(const int& NumSpecies,
                                    const int& NumGuilds,
                                    const std::string& GrowthForm,
                                    const std::string& HarvestForm,
                                    const std::string& CompetitionForm,
                                    const std::string& PredationForm)
{
    int m = 0;
    bool isLogistic    = (GrowthForm      == "Logistic");
    bool isHarvest     = (HarvestForm     == "Effort (qE)");
    bool isAlpha       = (CompetitionForm == "NO_K");
    bool isBetaGuilds  = (CompetitionForm == "AGG-PROD");
    bool isBetaSpecies = (CompetitionForm == "MS-PROD");
    bool isRho         = (PredationForm   != "Null");
    bool isHandling    = (PredationForm   == "Type II") || (PredationForm == "Type III");
    bool isExponent    = (PredationForm   == "Type III");
    int NumSpeciesOrGuilds = (isBetaGuilds) ? NumGuilds : NumSpecies;

    std::cout << "  Initial Absolute Biomass (B₀): ";
    for (int i=0; i<NumSpeciesOrGuilds; ++i) {
        std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
        ++m;
    }
    std::cout << "\n  Growth (r): ";
    for (int i=0; i<NumSpeciesOrGuilds; ++i) {
        std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
        ++m;
    }
    if (isLogistic) {
        std::cout << "\n  Carrying Capacity (K): ";
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
            ++m;
        }
    }
    if (isHarvest) {
        std::cout << "\n  Harvest: ";
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
            ++m;
        }
    }
    if (isAlpha) {
        std::cout << "\n  Competition (alpha):";
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            std::cout << "\n    ";
            for (int j=0; j<NumSpeciesOrGuilds; ++j) {
                std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
                ++m;
            }
        }
    }
    if (isBetaSpecies) {
        std::cout << "\n  Competition (beta(i)):";
        for (int i=0; i<NumSpecies; ++i) {
            std::cout << "\n    ";
            for (int j=0; j<NumSpecies; ++j) {
                std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
                ++m;
            }
        }
        std::cout << "\n  Competition (beta(G)):";
        for (int i=0; i<NumSpecies; ++i) {
            std::cout << "\n    ";
            for (int j=0; j<NumGuilds; ++j) {
                std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
                ++m;
            }
        }
    } else if (isBetaGuilds) {
        std::cout << "\n  Competition (beta(G)):";
        for (int i=0; i<NumGuilds; ++i) {
            std::cout << "\n    ";
            for (int j=0; j<NumGuilds; ++j) {
                std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
                ++m;
            }
        }
    }

    if (isRho) {
        std::cout << "\n  Predation (rho):";
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            std::cout << "\n    ";
            for (int j=0; j<NumSpeciesOrGuilds; ++j) {
                std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
                ++m;
            }
        }
    }
    if (isHandling) {
        std::cout << "\n  Handling Time (h):";
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
            std::cout << "\n    ";
            for (int j=0; j<NumSpeciesOrGuilds; ++j) {
                std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
                ++m;
            }
        }
    }
    if (isExponent) {
        std::cout << "\n  Predation Exponent (b):";
        std::cout << "\n    ";
        for (int j=0; j<NumSpeciesOrGuilds; ++j) {
            std::cout << "[" << m_ParameterRanges[m].first << "," << m_ParameterRanges[m].second << "] ";
            ++m;
        }
    }
    std::cout << std::endl;

}

void
BeesAlgorithm::loadInitBiomassParameterRanges(
        std::vector<std::pair<double,double> >& parameterRanges,
        nmfStructsQt::ModelDataStruct& dataStruct)
{
    std::pair<double,double> aPair;
    bool isCheckedInitBiomass = nmfUtils::isEstimateParameterChecked(dataStruct,"InitBiomass");

    // Always load initial biomass values
    for (unsigned species=0; species<dataStruct.InitBiomassMin.size(); ++species) {
        if (isCheckedInitBiomass) {
            aPair = std::make_pair(dataStruct.InitBiomassMin[species],
                                   dataStruct.InitBiomassMax[species]);
        } else {
            aPair = std::make_pair(dataStruct.InitBiomass[species], //-nmfConstantsMSSPM::epsilon,
                                   dataStruct.InitBiomass[species]); //+nmfConstantsMSSPM::epsilon);
        }
        parameterRanges.emplace_back(aPair);
    }
}


/*
 *
 * Create the parameters based upon the initial biomass and the 4 different
 * forms: growth, catch, competition, predation.
 */
void
BeesAlgorithm::initializeParameterRangesAndPatchSizes(nmfStructsQt::ModelDataStruct& theBeeStruct)
{
std::cout << "*** BeesAlgorithm::initializeParameterRangesAndPatchSizes ** " << std::endl;
    std::vector<std::pair<double,double> > parameterRanges;

    m_ParameterRanges.clear();
    m_PatchSizes.clear();
    loadInitBiomassParameterRanges(        parameterRanges, theBeeStruct);
    m_GrowthForm->loadParameterRanges(     parameterRanges, theBeeStruct);
    m_HarvestForm->loadParameterRanges(    parameterRanges, theBeeStruct);
    m_CompetitionForm->loadParameterRanges(parameterRanges, theBeeStruct);
    m_PredationForm->loadParameterRanges(  parameterRanges, theBeeStruct);
    m_ParameterRanges = parameterRanges;

    // Calculate the patch sizes for each parameter range
    for (unsigned int i=0;i<m_ParameterRanges.size();++i) {
        if (m_ParameterRanges[i].second == m_ParameterRanges[i].first) {
            m_PatchSizes.emplace_back(0);
        } else {
            m_PatchSizes.emplace_back(m_PatchSizePct*(m_ParameterRanges[i].second-m_ParameterRanges[i].first));
        }
    }

}



void
BeesAlgorithm::rescaleMinMax(const boost::numeric::ublas::matrix<double> &matrix,
                                   boost::numeric::ublas::matrix<double> &rescaledMatrix)
{
    int numYears   = matrix.size1();
    int numSpecies = matrix.size2();
    double den;
    double minVal;
    double maxVal;
    std::vector<double> minValues(numSpecies,0);
    std::vector<double> maxValues(numSpecies,0);
    std::vector<double> tmp(numYears,0);

    // Find min,max values for each column of matrix
    for (int species=0; species<numSpecies; ++species) {
        for (int time=0; time<numYears; ++time) {
            tmp[time] = matrix(time,species);
        }
        std::sort(tmp.begin(),tmp.end());
        minValues[species] = tmp.front();
        maxValues[species] = tmp.back();
    }

    // Rescale each column of the matrix with (x - min)/(max-min) formula.
    for (int species=0; species<numSpecies; ++species) {
        minVal = minValues[species];
        maxVal = maxValues[species];
        den    = maxVal - minVal;
        for (int time=0; time<numYears; ++time) {
            rescaledMatrix(time,species) = (matrix(time,species) - minVal) / den;  // min max normalization
        }
    }
}



void
BeesAlgorithm::rescaleMean(const boost::numeric::ublas::matrix<double> &matrix,
                                 boost::numeric::ublas::matrix<double> &rescaledMatrix)
{
    int numYears   = matrix.size1();
    int numSpecies = matrix.size2();
    double den;
    double minVal;
    double maxVal;
    double avgVal;
    std::vector<double> minValues(numSpecies,0);
    std::vector<double> maxValues(numSpecies,0);
    std::vector<double> avgValues(numSpecies,0);
    std::vector<double> tmp(numYears,0);

    // Find min,max values for each column of matrix
    for (int species=0; species<numSpecies; ++species) {
        avgVal = 0;
        for (int time=0; time<numYears; ++time) {
            tmp[time] = matrix(time,species);
            avgVal += tmp[time];
        }
        avgVal /= numYears;
        std::sort(tmp.begin(),tmp.end());
        minValues[species] = tmp.front();
        maxValues[species] = tmp.back();
        avgValues[species] = avgVal;
    }

    // Rescale each column of the matrix with (x - min)/(max-min) formula.
    for (int species=0; species<numSpecies; ++species) {
        minVal = minValues[species];
        maxVal = maxValues[species];
        den    = maxVal - minVal;
        avgVal = avgValues[species];
        for (int time=0; time<numYears; ++time) {
            rescaledMatrix(time,species) = (matrix(time,species) - avgVal) / den; // mean normalization
        }
    }
}


void
BeesAlgorithm::rescaleZScore(const boost::numeric::ublas::matrix<double> &matrix,
                                   boost::numeric::ublas::matrix<double> &rescaledMatrix)
{
    int numYears   = matrix.size1();
    int numSpecies = matrix.size2();
    double avgVal;
    double val;
    double sigVal;
    double diff;
    std::vector<double> avgValues(numSpecies,0);
    std::vector<double> sigma(numSpecies,0);

    // Find min,max values for each column of matrix
    for (int species=0; species<numSpecies; ++species) {
        avgVal = 0;
        for (int time=0; time<numYears; ++time) {
            avgVal += matrix(time,species);
        }
        avgVal /= numYears;
        avgValues[species] = avgVal;
    }

    // Find standard deviation
    for (int species=0; species<numSpecies; ++species) {
        val = 0;
        for (int time=0; time<numYears; ++time) {
            diff = (matrix(time,species) - avgVal);
            val += diff*diff;
        }
        val /= numYears;
        sigma[species] = std::sqrt(val);
    }

    // Rescale each column of the matrix with (x - min)/(max-min) formula.
    for (int species=0; species<numSpecies; ++species) {
        avgVal = avgValues[species];
        sigVal = sigma[species];
        for (int time=0; time<numYears; ++time) {
            rescaledMatrix(time,species) = (matrix(time,species) - avgVal) / sigVal; // standardization or z score normalization
        }
    }
}

void
BeesAlgorithm::extractInitBiomass(const std::vector<double>& parameters,
                                  int&                       startPos,
                                  std::vector<double>&       initBiomass)
{
    int numInitBiomassParameters = m_BeeStruct.InitBiomassMin.size();

    for (int i=startPos; i<numInitBiomassParameters; ++i) {
        initBiomass.emplace_back(parameters[i]);
    }
    startPos = numInitBiomassParameters;
}

void
BeesAlgorithm::extractGrowthParameters(const std::vector<double>& parameters,
                                       int&                       startPos,
                                       std::vector<double>&       growthRate,
                                       std::vector<double>&       carryingCapacity,
                                       double&                    systemCarryingCapacity)
{
    if ((m_GrowthForm != nullptr)) {
        m_GrowthForm->extractParameters(parameters,startPos,growthRate,
                                        carryingCapacity,systemCarryingCapacity);
    }
}


void
BeesAlgorithm::extractHarvestParameters(const std::vector<double>& parameters,
                                        int& startPos,
                                        std::vector<double>& catchabilityRate)
{
    if ((m_HarvestForm != nullptr)) {
        m_HarvestForm->extractParameters(parameters,startPos,catchabilityRate);
    }
}

void
BeesAlgorithm::extractCompetitionParameters(const std::vector<double>& parameters,
                                            int& startPos,
                                            boost::numeric::ublas::matrix<double>& alpha,
                                            boost::numeric::ublas::matrix<double>& betaSpecies,
                                            boost::numeric::ublas::matrix<double>& betaGuilds,
                                            boost::numeric::ublas::matrix<double>& betaGuildsGuilds)
{
    if ((m_CompetitionForm != nullptr)) {
        m_CompetitionForm->extractParameters(parameters,startPos,alpha,betaSpecies,betaGuilds,betaGuildsGuilds);
    }
}

void
BeesAlgorithm::extractPredationParameters(const std::vector<double>& parameters,
                                          int& startPos,
                                          boost::numeric::ublas::matrix<double>& predation)
{
    if ((m_PredationForm != nullptr)) {
        m_PredationForm->extractPredationParameters(parameters,startPos,predation);
    }
}

void
BeesAlgorithm::extractHandlingParameters(const std::vector<double>& parameters,
                                         int& startPos,
                                         boost::numeric::ublas::matrix<double>& handling)
{
    if ((m_PredationForm != nullptr)) {
        m_PredationForm->extractHandlingParameters(parameters,startPos,handling);
    }
}

void
BeesAlgorithm::extractExponentParameters(const std::vector<double>& parameters,
                                         int& startPos,
                                         std::vector<double>& exponents)
{
    if ((m_PredationForm != nullptr)) {
        m_PredationForm->extractExponentParameters(parameters,startPos,exponents);
    }
}

void
BeesAlgorithm::extractSurveyQParameters(const std::vector<double>& parameters,
                              int&                       startPos,
                              std::vector<double>&       surveyQ)
{
    int numSurveyQParameters = m_BeeStruct.SurveyQMin.size();

    for (int i=startPos; i<numSurveyQParameters; ++i) {
        surveyQ.emplace_back(parameters[i]);
    }
    startPos += numSurveyQParameters;
}

double
BeesAlgorithm::evaluateObjectiveFunction(const std::vector<double> &parameters)
{
    bool   isAggProd = (m_BeeStruct.CompetitionForm == "AGG-PROD");
    double estBiomassVal;
    double growthTerm;
    double harvestTerm;
    double competitionTerm;
    double predationTerm;
    double systemCarryingCapacity;
    double guildK;
    double fitness=0;
    int startPos=0;
    int timeMinus1;
    int NumYears   = m_BeeStruct.RunLength+1;
    int NumSpecies = m_BeeStruct.NumSpecies;
    int NumGuilds  = m_BeeStruct.NumGuilds;
    int guildNum;
    int NumSpeciesOrGuilds = (isAggProd) ? NumGuilds : NumSpecies;
    std::vector<double> initBiomass;
    std::vector<double> growthRate;
    std::vector<double> carryingCapacity;
    std::vector<double> guildCarryingCapacity;
    std::vector<double> exponent;
    std::vector<double> catchabilityRate;
    boost::numeric::ublas::matrix<double> estBiomassSpecies;
    boost::numeric::ublas::matrix<double> estBiomassGuilds;
    boost::numeric::ublas::matrix<double> estBiomassRescaled;
    boost::numeric::ublas::matrix<double> obsBiomassBySpeciesOrGuildsRescaled;
    boost::numeric::ublas::matrix<double> competitionAlpha;
    boost::numeric::ublas::matrix<double> competitionBetaSpecies;
    boost::numeric::ublas::matrix<double> competitionBetaGuilds;
    boost::numeric::ublas::matrix<double> competitionBetaGuildsGuilds;
    boost::numeric::ublas::matrix<double> predation;
    boost::numeric::ublas::matrix<double> handling;
    nmfUtils::initialize(estBiomassSpecies,                   NumYears,           NumSpeciesOrGuilds);
    nmfUtils::initialize(estBiomassGuilds,                    NumYears,           NumGuilds);
    nmfUtils::initialize(estBiomassRescaled,                  NumYears,           NumSpeciesOrGuilds);
    nmfUtils::initialize(obsBiomassBySpeciesOrGuildsRescaled, NumYears,           NumSpeciesOrGuilds);
    nmfUtils::initialize(competitionAlpha,                    NumSpeciesOrGuilds, NumSpeciesOrGuilds);
    nmfUtils::initialize(competitionBetaSpecies,              NumSpecies,         NumSpecies);
    nmfUtils::initialize(competitionBetaGuilds,               NumSpeciesOrGuilds, NumGuilds);
    nmfUtils::initialize(competitionBetaGuildsGuilds,         NumGuilds,          NumGuilds);
    nmfUtils::initialize(predation,                           NumSpeciesOrGuilds, NumSpeciesOrGuilds);
    nmfUtils::initialize(handling,                            NumSpeciesOrGuilds, NumSpeciesOrGuilds);

//std::cout << "num params: " << parameters.size() << std::endl;
    // Load the parameters into their respective data structures for use in the objective function.
    extractInitBiomass(parameters,startPos,initBiomass);
//std::cout << "start pos: " << startPos << std::endl;
//std::cout << "num parameters: " << parameters.size() << std::endl;
    m_GrowthForm->extractParameters(parameters,startPos,growthRate,
                                    carryingCapacity,systemCarryingCapacity);
    m_HarvestForm->extractParameters(parameters,startPos,catchabilityRate);
    m_CompetitionForm->extractParameters(parameters,startPos,competitionAlpha,
                                         competitionBetaSpecies,competitionBetaGuilds,
                                         competitionBetaGuildsGuilds);
    m_PredationForm->extractPredationParameters(parameters,startPos,predation);
    m_PredationForm->extractHandlingParameters(parameters,startPos,handling);
    m_PredationForm->extractExponentParameters(parameters,startPos,exponent);
//std::cout << "last start pos: " << startPos << std::endl;

    // guildCarryingCapacity is carrying capacity for the guild the species is a member of

    // Calculate carrying capacity for all guilds
    systemCarryingCapacity = 0;
//std::cout << "NumGuilds: " << NumGuilds << std::endl;
    for (int i=0; i<NumGuilds; ++i) {
        guildK = 0;
//std::cout << "m_GuildSpecies[i].size(): " << m_GuildSpecies[i].size() << std::endl;
//std::cout << "carryingCapacity.size(): " << carryingCapacity.size() << std::endl;

        for (unsigned j=0; j<m_GuildSpecies[i].size(); ++j) {
//std::cout << "m_GuildSpecies[" << i << "][" << j << "]: " << m_GuildSpecies[i][j] << std::endl;

            guildK += carryingCapacity[m_GuildSpecies[i][j]];
//std::cout << "carryingCapacity[m_GuildSpecies[i][j]]: " << carryingCapacity[m_GuildSpecies[i][j]] << std::endl;

            systemCarryingCapacity += guildK;
        }
        guildCarryingCapacity.push_back(guildK);
    }

    // Evaluate the objective function for all years and species or guilds and put
    // result in matrix
    for (int i=0; i<NumSpeciesOrGuilds; ++i) {
        estBiomassSpecies(0,i) = m_ObsBiomassBySpeciesOrGuilds(0,i);
    }

    for (int i=0; i<NumGuilds; ++i) {
        estBiomassGuilds(0,i)  = m_ObsBiomassByGuilds(0,i); // Remember there's only initial guild biomass data.
    }

    bool isCheckedInitBiomass = nmfUtils::isEstimateParameterChecked(m_BeeStruct,"InitBiomass");

//std::cout << "num years:: " << NumYears << std::endl;
    for (int time=1; time<NumYears; ++time) {
//std::cout << "year :: " << time << std::endl;
        timeMinus1 = time - 1;
        for (int i=0; i<NumSpeciesOrGuilds; ++i) {
//std::cout << "i :: " << i << std::endl;
            // Find guild that speciesNum is in
            guildNum = m_GuildNum[i];

            if (isCheckedInitBiomass) {
                if (timeMinus1 == 0) {
                    estBiomassVal = initBiomass[i];
                } else {
                    estBiomassVal = estBiomassSpecies(timeMinus1,i);
                }
            } else {
                estBiomassVal = estBiomassSpecies(timeMinus1,i);
            }
//          estBiomassVal = estBiomassSpecies(timeMinus1,i);

//std::cout << " estBiomassVal: " << estBiomassVal << std::endl;
//std::cout << " growthRate size: " << growthRate.size() << std::endl;
//std::cout << " carryingCapacity size: " << carryingCapacity.size() << std::endl;

            growthTerm      = m_GrowthForm->evaluate(i,estBiomassVal,
                                                     growthRate,carryingCapacity);
            harvestTerm     = m_HarvestForm->evaluate(timeMinus1,i,
                                                      m_Catch,m_Effort,m_Exploitation,
                                                      estBiomassVal,catchabilityRate);
//std::cout << "guild cc: " << guildCarryingCapacity[guildNum] << std::endl;
//std::cout << "system cc: " << systemCarryingCapacity << std::endl;
            competitionTerm = m_CompetitionForm->evaluate(
                                   timeMinus1,i,estBiomassVal,
                                   systemCarryingCapacity,
                                   growthRate,
                                   guildCarryingCapacity[guildNum],
                                   competitionAlpha,
                                   competitionBetaSpecies,
                                   competitionBetaGuilds,
                                   competitionBetaGuildsGuilds,
                                   estBiomassSpecies,
                                   estBiomassGuilds);
            predationTerm   = m_PredationForm->evaluate(
                                   timeMinus1,i,
                                   predation,handling,exponent,
                                   estBiomassSpecies,estBiomassVal);

            estBiomassVal  += growthTerm - harvestTerm - competitionTerm - predationTerm;
//std::cout << "estBiomassVal: " << estBiomassVal <<
//             ", g: " << growthTerm <<
//             ", h: " << harvestTerm <<
//             ", c: " << competitionTerm <<
//             ", p: " << predationTerm << std::endl;
//std::cout << "estBiomassVal: " << estBiomassVal << std::endl;
if (estBiomassVal < 0) {
 estBiomassVal = 0;
}
            if ((estBiomassVal < 0) || (std::isnan(std::fabs(estBiomassVal)))) {
//std::cout << "*** Returning... *** " << std::endl;
                return m_DefaultFitness;
            }
            estBiomassSpecies(time,i) = estBiomassVal;

            // update estBiomassGuilds for next time step
            for (int i=0; i<NumGuilds; ++i) {
                for (unsigned j=0; j<m_GuildSpecies[i].size(); ++j) {
                    estBiomassGuilds(time,i) += estBiomassSpecies(time,m_GuildSpecies[i][j]);
                }
            }
        }
    }

    // Scale the data
    if (m_Scaling == "Min Max") {
        rescaleMinMax(estBiomassSpecies, estBiomassRescaled);
        rescaleMinMax(m_ObsBiomassBySpeciesOrGuilds, obsBiomassBySpeciesOrGuildsRescaled);
    } else if (m_Scaling == "Mean") {
        rescaleMean(estBiomassSpecies, estBiomassRescaled);
        rescaleMean(m_ObsBiomassBySpeciesOrGuilds, obsBiomassBySpeciesOrGuildsRescaled);
    } else if (m_Scaling == "Z-Score") {
        rescaleZScore(estBiomassSpecies, estBiomassRescaled);
        rescaleZScore(m_ObsBiomassBySpeciesOrGuilds, obsBiomassBySpeciesOrGuildsRescaled);
    } else {
//        std::cout << "Error: No Scaling Algorithm detected. Defaulting to Min Max." << std::endl;
        rescaleMinMax(estBiomassSpecies, estBiomassRescaled);
        rescaleMinMax(m_ObsBiomassBySpeciesOrGuilds, obsBiomassBySpeciesOrGuildsRescaled);
    }

    // Calculate fitness using the appropriate objective criterion
    if (m_BeeStruct.ObjectiveCriterion == "Least Squares") {

        fitness =  nmfUtilsStatistics::calculateSumOfSquares(
                    estBiomassRescaled,
                    obsBiomassBySpeciesOrGuildsRescaled);

    } else if (m_BeeStruct.ObjectiveCriterion == "Model Efficiency") {

        // Negate the MEF here since the ranges is from -inf to 1, where 1 is best.  So we negate it,
        // then minimize that, and then negate and plot the resulting value.
        fitness = -nmfUtilsStatistics::calculateModelEfficiency(
                    estBiomassRescaled,
                    obsBiomassBySpeciesOrGuildsRescaled);
    } else if (m_BeeStruct.ObjectiveCriterion == "Maximum Likelihood") {
        // The maximum likelihood calculations must use the unscaled data or else the
        // results will be incorrect.
        fitness =  nmfUtilsStatistics::calculateMaximumLikelihoodNoRescale(
                    estBiomassSpecies,
                    m_ObsBiomassBySpeciesOrGuilds);
    }

    return fitness;
}


std::unique_ptr<Bee>
BeesAlgorithm::createRandomBee(bool doWhileLoop, std::string& errorMsg)
{
    bool foundAPotentialBee = false;
    bool timesUp = false;
    int timeDiff;
    double minVal;
    double maxVal;
    double fitness;
    QDateTime startTime = nmfUtilsQt::getCurrentTime();
    QDateTime endTime;
    std::vector<double> NullParameters = {};
    std::vector<double> parameters(m_BeeStruct.TotalNumberParameters,0.0);

//std::cout << "--> Num Parameters: " << m_BeeStruct.TotalNumberParameters << std::endl;
    while (! foundAPotentialBee) {
        for (int i=0; i<m_BeeStruct.TotalNumberParameters; ++i) {
            minVal = m_ParameterRanges[i].first;
            maxVal = m_ParameterRanges[i].second;
//std::cout << "--> range: " << i << "  [" << minVal << "," << maxVal << "] ";
            parameters[i] = (maxVal == minVal) ? minVal :
                             minVal+(maxVal-minVal)*(nmfUtils::getRandomNumber(m_Seed,0.0,1.0));
//std::cout << "--> " << parameters[i] << std::endl;
        }
        fitness = evaluateObjectiveFunction(parameters);
//std::cout << "--> fitness: " << fitness << std::endl;
        endTime   = nmfUtilsQt::getCurrentTime();
        timeDiff = int(startTime.msecsTo(endTime))*1000.0; // microseconds
//std::cout << "--> timeDiff: " << timeDiff/1000000.0 << "\n" << std::endl;
        timesUp = (timeDiff/1000000.0 > kTimeToSpendSearching);

        foundAPotentialBee = (fitness != m_DefaultFitness) || doWhileLoop || timesUp;
    }
    if (timesUp) {
        std::cout << "Fitness found: " << fitness << std::endl;
        errorMsg = "\nError: Parameter space too large. Please refine one or more parameter ranges or decrease a parameter.";
        return std::make_unique<Bee>(m_NullFitness,NullParameters);
    }

    return std::make_unique<Bee>(fitness,parameters);
}


std::unique_ptr<Bee>
BeesAlgorithm::createNeighborhoodBee(std::vector<double> &bestSiteParameters)
{
    double val;
    double fitness;
    double patchSize;
    //double rval = rand()/double(RAND_MAX);
    double rval;
    std::vector<double> parameters = {};
//std::cout << "rval: " << rval << std::endl;
    for (unsigned int i=0; i<bestSiteParameters.size(); ++i) {
        patchSize = m_PatchSizes[i];
//std::cout << i << ", " << patchSize << std::endl;
        val = bestSiteParameters[i];
        rval = nmfUtils::getRandomNumber(m_Seed,0.0,1.0);
        if (patchSize > 0) {
            val = (rval < 0.5) ? (val+rval*patchSize) : (val-rval*patchSize);
            // In c++17, use...
            //std::clamp(val,m_parameterRanges[i].first,m_parameterRanges[i].second);
            val = (val < m_ParameterRanges[i].first)  ? m_ParameterRanges[i].first  : val;
            val = (val > m_ParameterRanges[i].second) ? m_ParameterRanges[i].second : val;
        }
        parameters.emplace_back(val);
    }
    fitness = evaluateObjectiveFunction(parameters);

  return std::move(std::make_unique<Bee>(fitness,parameters));
}


std::unique_ptr<Bee>
BeesAlgorithm::searchNeighborhoodForBestBee(std::unique_ptr<Bee> bestSite,
                                            int &neighborhoodSize)
{
    std::unique_ptr<Bee> bee;
    std::vector<std::unique_ptr<Bee> > neighborhoodBees;
    std::vector<double> bestSiteParameters = bestSite->getParameters();

    for (int i=0; i<neighborhoodSize; ++i) {
        bee = createNeighborhoodBee(bestSiteParameters);
        neighborhoodBees.emplace_back(std::move(bee));
    }

    std::sort(neighborhoodBees.begin(),neighborhoodBees.end(),beesCompareLT());

    return std::move(neighborhoodBees.front());
}


std::unique_ptr<Bee>
BeesAlgorithm::searchParameterSpaceForBestBee(int &RunNum,
                                              int& subRunNum,
                                              std::string& errorMsg)
{
    bool done = false;
    int currentGeneration=0;
    int neighborhoodSize;
    int genNum;
    int numScoutBees;
    int numParameters  = m_BeeStruct.TotalNumberParameters;
    int numTotalBees   = m_BeeStruct.BeesNumTotal;
    int numEliteBees   = m_BeeStruct.BeesNumElite;
    int numOtherBees   = m_BeeStruct.BeesNumOther;
    int numBestSites   = m_BeeStruct.BeesNumBestSites;
    int numEliteSites  = m_BeeStruct.BeesNumEliteSites;
    int maxGenerations = m_BeeStruct.BeesMaxGenerations;
    std::unique_ptr<Bee> bestBee;
    std::vector<std::unique_ptr<Bee> > totalBeePopulation;
    std::vector<std::unique_ptr<Bee> > nextGenerationBees;
    std::vector<std::unique_ptr<Bee> > scoutBees;
    std::vector<std::unique_ptr<Bee> > bestSites;
    std::unique_ptr<Bee> theBestBee;
    std::string MSSPMName;
    double bestFitness;
    double bestFitnessInPopulation;
    double bestBeesFitness;
    int unused;

std::cout << "Searching parameter space for initial bees..." << std::endl;

    theBestBee = createRandomBee(false,errorMsg);
std::cout << "Found a bee" << std::endl;

    if (theBestBee->getFitness() == m_NullFitness) {
        return theBestBee;
    }
    for (int i=0; i<numTotalBees; ++i) {
std::cout << "Creating bee: " << i << std::endl;
        totalBeePopulation.emplace_back(createRandomBee(false,errorMsg));
    }
std::cout << "Found initial bees." << std::endl;

    while (! done) {

        std::sort(totalBeePopulation.begin(),
                  totalBeePopulation.end(),
                  beesCompareLT());

        bestFitnessInPopulation = totalBeePopulation[0]->getFitness();
        bestBeesFitness         = theBestBee->getFitness();

        if ((bestBeesFitness == m_DefaultFitness) ||
                isABetterFitness(bestFitnessInPopulation,bestBeesFitness))
        {
            theBestBee->setFitness(bestFitnessInPopulation);
            theBestBee->setParameters(totalBeePopulation[0]->getParameters());
        }

        // Find the best sites
        bestSites.clear();
        for (int i=0; i<numBestSites; ++i) {
            bestSites.emplace_back(std::move(totalBeePopulation[i]));
        }

        // For each best bee, calculate the neighborhood size in terms of bees,
        // find those bees in each neighborhood and add only the best one to next_gen
        nextGenerationBees.clear();
        for (int i=0; i<numBestSites; ++i) {
            neighborhoodSize = (i < numEliteSites) ? numEliteBees : numOtherBees;
            nextGenerationBees.emplace_back(searchNeighborhoodForBestBee(std::move(bestSites[i]),
                                                                         neighborhoodSize));
        }

        // Now find the rest of the bees that make up the total number of bees as we'll
        // use them for new scouts.
        numScoutBees = numTotalBees - numBestSites;
        scoutBees.clear();
        for (int i=0; i<numScoutBees; ++i) {
            scoutBees.emplace_back(createRandomBee(false,errorMsg));
//std::cout << "scout bee: " << i << ": " << scoutBees[i]->getFitness() << std::endl;
        }

        totalBeePopulation.clear();
        for (int i=0; i<numBestSites; ++i) {
            totalBeePopulation.emplace_back(std::move(nextGenerationBees[i]));
        }
        for (int i=0; i<numScoutBees; ++i) {
            totalBeePopulation.emplace_back(std::move(scoutBees[i]));
        }

        // Decrease the patch size, so new neighborhoods get smaller and smaller
        for (int i=0; i<numParameters; ++i) {
           m_PatchSizes[i] *= 0.95;
        }

        ++currentGeneration;

        // Done when loop has gone through maxGenerations 
        done = (currentGeneration >= maxGenerations);
//std::cout << "Generation: " <<  currentGeneration << ", Fitness: " << theBestBee->getFitness() << std::endl;

        MSSPMName = "Run " + std::to_string(RunNum) +
                    "-"    + std::to_string(subRunNum);
        bestFitness = theBestBee->getFitness();

        unused = 0;
        genNum = currentGeneration - 1;
        WriteCurrentLoopFile(MSSPMName,
                     genNum,
                     bestFitness,
                     unused);

        if (StoppedByUser()) {
            std::cout << "BeesAlgorithm StoppedByUser" << std::endl;
            done = true;
        }

    }
    ++genNum;
    WriteCurrentLoopFile(MSSPMName,
                 genNum,
                 bestFitness,
                 unused);
    return std::move(theBestBee);
}


bool
BeesAlgorithm::isABetterFitness(double& bestFitnessInPopulation,
                                double& bestBeesFitness)
{

    return (bestFitnessInPopulation < bestBeesFitness);  // Minimize the fitness

}

bool
BeesAlgorithm::StoppedByUser()
{
    std::string cmd;
    std::ifstream inputFile(nmfConstantsMSSPM::MSSPMStopRunFile);
    if (inputFile) {
        std::getline(inputFile,cmd);
    }
    inputFile.close();

    return (cmd == "StoppedByUser");
}

void
BeesAlgorithm::WriteCurrentLoopFile(std::string &MSSPMName,
                                   int         &NumGens,
                                   double      &BestFitness,
                                   int         &NumGensSinceBestFit)
{
    double adjustedBestFitness; // May need negating if ObjCrit is Model Efficiency
    std::ofstream outputFile(nmfConstantsMSSPM::MSSPMProgressChartFile,
                             std::ios::out|std::ios::app);

    adjustedBestFitness = BestFitness;
    //
    // Model Efficiency is to be maximized instead of minimized.  The
    // best value is 1. Since the code is set up to minimize for Least
    // Squares, I just negated the fitness and ran the minimization code.
    // Now, I just need to negate the fitness again so the plot will
    // show the fitness approaching +1.
    //
    if (m_BeeStruct.ObjectiveCriterion == "Model Efficiency") {
        adjustedBestFitness = -adjustedBestFitness;
    }

    outputFile << MSSPMName   << ", "
               << NumGens     << ", "
               << adjustedBestFitness << ", "
               << NumGensSinceBestFit << std::endl;

    outputFile.close();
}

void
BeesAlgorithm::printBee(double &fitness,
                        std::vector<double> &parameters)
{
    int numParameters = parameters.size();

    std::cout << "Estimated Parameters: " << " ";
    std::cout << fitness;
    for (int i=0; i<numParameters; ++i) {
        std::cout << " " << parameters[i];
    }
    std::cout << std::endl;
}

int
BeesAlgorithm::calculateActualNumEstParameters()
{
    int numParameters = 0;

    for (unsigned i=0; i<m_ParameterRanges.size(); ++i) {
        if (m_ParameterRanges[i].first != m_ParameterRanges[i].second) {
            ++numParameters;
        }
    }
    return numParameters;
}

bool
BeesAlgorithm::estimateParameters(double &bestFitness,
                                  std::vector<double> &bestParameters,
                                  int &RunNum,
                                  int &subRunNum,
                                  std::string& errorMsg)
{
    bool ok = false;
    std::unique_ptr<Bee> bestBee;

    m_DefaultFitness =  99999;
    m_NullFitness    = -999.9;

    bestBee = searchParameterSpaceForBestBee(RunNum,subRunNum,errorMsg);

    if (bestBee->getFitness() != m_NullFitness) {
        bestFitness    = bestBee->getFitness();
        bestParameters = bestBee->getParameters();
        ok = true;
    }

    return ok;
}
