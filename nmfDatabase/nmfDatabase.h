
/**
 *
 * @file nmfDatabase.h
 * @date Nov 21, 2016
 * @author Ronald Klasky
 *
 * Public Domain Notice
 * National Oceanic And Atmospheric Administration
 *
 * This software is a "United States Government Work" under the terms of the
 * United States Copyright Act.  It was written as part of the author's official
 * duties as a United States Government employee/contractor and thus cannot be copyrighted.
 * This software is freely available to the public for use. The National Oceanic
 * And Atmospheric Administration and the U.S. Government have not placed any
 * restriction on its use or reproduction.  Although all reasonable efforts have
 * been taken to ensure the accuracy and reliability of the software and data,
 * the National Oceanic And Atmospheric Administration and the U.S. Government
 * do not and cannot warrant the performance or results that may be obtained
 * by using this software or data. The National Oceanic And Atmospheric
 * Administration and the U.S. Government disclaim all warranties, express
 * or implied, including warranties of performance, merchantability or fitness
 * for any particular purpose.
 *
 * Please cite the author(s) in any work or product based on this material.
 *
 * @brief This file contains the database API implementation (currently implemented for MySQL).
 */


#ifndef NMFDATABASE_H_
#define NMFDATABASE_H_

#include "nmfLogger.h"
#include "nmfUtilsQt.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iterator>
#include <exception>
#include <tuple>

/*
#include "fileIO.h"
#include "mySqlIO.h"
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
*/
#include <boost/numeric/ublas/matrix.hpp>

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QString>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QSqlError>


/**
 * @brief Allows user to interact with MySQL database
 *
 * This class allows the user to interact with the MySQL database stored locally
 * on the user's machine. The user can create or delete databases or tables and
 * read from or write to specific database tables. Before using this class the
 * user must assure that MySQL is installed and properly working on the host machine.
 *
 */
class nmfDatabase {

private:
    QString m_dbName;
    std::map<QString, void(*)(
            QString &table,
            QString &cmd
            )> m_FunctionMap;

public:

    /**
     * @brief Class constructor
     */
    nmfDatabase();

    /**
     * @brief Opens/prepares a database for I/O.
     * @param hostName - The name of the host computer
     * @param userName - The user name
     * @param password - The MySQL password for the current user
     * @param errorMsg - If an error was encountered it'll be placed here, otherwise it'll be empty
     * @return True if database was opened successfully, otherwise false
     */
    bool nmfOpenDatabase(
            const std::string &hostName,
            const std::string &userName,
            const std::string &password,
            std::string&       errorMsg);
    bool getForecastCatch(
            QWidget*           Widget,
            nmfLogger*         Logger,
            const std::string& ForecastName,
            const int&         NumSpecies,
            const int&         RunLength,
            std::string&       Algorithm,
            std::string&       Minimizer,
            std::string&       ObjectiveCriterion,
            std::string&       Scaling,
            std::vector<boost::numeric::ublas::matrix<double> >& ForecastCatch);
    /**
     * @brief Closes the database
     * @param none
     * @return none
     */
    void nmfCloseDatabase();
    std::string nmfLastError();

    void nmfStartTransaction();

    void nmfSaveDatabase();
    std::string nmfGetCurrentDatabase();
    double checkForValues(
            const int& index,
            const int& numValues,
            const std::vector<double>& vec);
    std::vector<std::string> nmfGetDatabaseNames();
    bool clearTable(nmfLogger*   Logger,
                    const std::string& TableName);
    bool getAllSpecies(nmfLogger*  logger,
                       std::vector<std::string>& species);
    bool getAllGuilds(nmfLogger*  logger,
                      std::vector<std::string>& guilds);
    bool getForecastInfo(
            const std::string TableName,
            const std::string ForecastName,
            int&         RunLength,
            int&         StartForecastYear,
            std::string& Algorithm,
            std::string& Minimizer,
            std::string& ObjectiveCriterion,
            std::string& Scaling,
            int&         NumRuns);
    bool getForecastBiomass(
            QWidget*           Widget,
            nmfLogger*         Logger,
            const std::string& ForecastName,
            const int&         NumSpecies,
            const int&         RunLength,
            std::string&       Algorithm,
            std::string&       Minimizer,
            std::string&       ObjectiveCriterion,
            std::string&       Scaling,
            std::vector<boost::numeric::ublas::matrix<double> >& ForecastBiomass);
    bool getForecastBiomassMonteCarlo(
            QWidget*           Widget,
            nmfLogger*         Logger,
            const std::string& ForecastName,
            const int&         NumSpecies,
            const int&         RunLength,
            const int&         NumRuns,
            std::string&       Algorithm,
            std::string&       Minimizer,
            std::string&       ObjectiveCriterion,
            std::string&       Scaling,
            std::vector<boost::numeric::ublas::matrix<double> >& ForecastBiomassMonteCarlo);
    bool getSpeciesData(nmfLogger*   logger,
                        std::string  species,
                        int&         MinAge,
                        int&         MaxAge,
                        int&         FirstYear,
                        int&         LastYear,
                        float&       MinLength,
                        float&       MaxLength,
                        int&         NumLengthBins);
    bool getTimeSeriesData(
            QWidget*           Widget,
            nmfLogger*         Logger,
            const std::string& ProjectSettingsConfig,
            const std::string  MohnsRhoLabel,
            const std::string  ForecastName,
            const std::string& TableName,
            const int&         NumSpecies,
            const int&         RunLength,
            boost::numeric::ublas::matrix<double>& TableData);
    std::vector<std::string> nmfGetTableNames();
    int getSpeciesIndex(std::string SpeciesName);
    bool getForecastMonteCarloParameters(
            QWidget*             widget,
            nmfLogger*           logger,
            const std::string&   Species,
            const std::string&   ForecastName,
            std::string&         Algorithm,
            std::string&         Minimizer,
            std::string&         ObjectiveCriterion,
            std::string&         Scaling,
            QStringList&         HoverData);
    bool updateForecastMonteCarloParameters(
            QWidget*             widget,
            nmfLogger*           logger,
            const std::string&   ForecastName,
            std::string&         Algorithm,
            std::string&         Minimizer,
            std::string&         ObjectiveCriterion,
            std::string&         Scaling,
            QStringList&         Species,
            int&                 RunNumber,
            std::vector<double>& GrowthRandomValues,
            std::vector<double>& CarryingCapacityRandomValues,
            std::vector<double>& CatchabilityRandomValues,
            std::vector<double>& ExponentRandomValues,
            std::vector<double>& CompetitionAlphaRandomValues,
            std::vector<double>& CompetitionBetaSpeciesRandomValues,
            std::vector<double>& CompetitionBetaGuildsRandomValues,
            std::vector<double>& CompetitionBetaGuildsGuildsRandomValues,
            std::vector<double>& PredationRandomValues,
            std::vector<double>& HandlingRandomValues,
            std::vector<double>& HarvestRandomValues);

    /**
     * @brief Closes the Qt SQL database connection
     */
    void nmfDeleteConnection();

    /**
     * @brief Sets the database connection by using the passed in connection name
     * @param name - database connection name (i.e., something like: qt_sql_default_connection)
     */
    void nmfSetConnectionByName(QString name);

    void nmfSetDatabase(std::string newDatabaseName);

    /**
     * @brief Executes a database query.
     * @param query - the database query string
     * @param fields - the database fields to be queried
     * @param setNullToBlank - uses empty string if table field is NULL, else uses 0
     * @return none
     */
    std::map< std::string, std::vector< std::string> >
    nmfQueryDatabase(
            const std::string &query,
            const std::vector<std::string> &fields,
            const bool setNullToBlank=false);

    /**
     * @brief Executes a non-query database command (i.e. delete).
     * @param query - the database query string
     * @return std::string error msg (if any)
     */
    std::string nmfUpdateDatabase(std::string qry);

    /**
     * @brief Deletes all records from a table.
     * @param table - the database table to clear
     * @param MSVPAName - the name of the MSVPA file
     * @return bool - flag denoting if the records were deleted successfully
     */
    bool nmfDeleteRecordsFromTable(
            std::string table,
            std::string MSVPAName);

    bool nmfDeleteRecordsFromTable(std::string table,
           std::string MSVPAName,
           std::string ForecastName,
           std::string Scenario);

    std::map<std::string, int>
    nmfQueryInitFields(
            const std::string &table,
            const std::string &MSVPAName);

    std::map<std::string, std::vector<std::string> >
    nmfQueryPredatorPreyFields(
            const std::string &table,
            const std::string &MSVPAName);

    bool nmfGetInitialData(std::string MSVPAName,
        int &NPreds, int &NPrey, int &NOtherFood, int &NOtherPreds,
        int &FirstYear, int &LastYear, int &NSeas, int &NMSVPASpe, int &NYears,
        std::string &FirstYearStr, std::string &LastYearStr);


    /**
     * @brief Executes a database query of 4 commonly requested age-related fields
     * @param table - name of database table
     * @param speciesIndex - species index value
     * @return std::tuple of integers containing the age fields: MaxAge, MinCatAge, MaxCatAge, PlusClass
     */
    std::tuple<int, int, int, int> nmfQueryAgeFields(const std::string &table, const int &speciesIndex);

    void nmfGetMSVPAInitData(std::string &MSVPAName,
       int &MSVPA_FirstYear,
       int &MSVPA_LastYear,
       int &MSVPA_NYears,
       int &MSVPA_NSeasons);

    void nmfQueryForecastAgeCatchData(
            const std::string &MSVPAName,
            const std::string &ForecastName,
            const std::string &ScenarioName,
            const std::string &species,
            const int &Year,
            const int &Nage,
            const int &i,
            boost::numeric::ublas::matrix<double> &Catch);

    void nmfQueryForecastMortalityData(
            const std::string &MSVPAName,
            const std::string &ForecastName,
            const std::string &ScenarioName,
            const std::string &species,
            const int &Year,
            const int &Nage,
            const int &i,
            boost::numeric::ublas::matrix<double> &FatAge,
            boost::numeric::ublas::matrix<double> &M1atAge,
            boost::numeric::ublas::matrix<double> &M2atAge);

    void nmfQueryForecastWeightAtAgeData(
            const std::string &MSVPAName,
            const std::string &ForecastName,
            const std::string &ScenarioName,
            const std::string &species,
            const int &Year,
            const int &Nage,
            const int &i,
            boost::numeric::ublas::matrix<double> &WtAtAge);

    bool nmfQueryWeightAtAgeData(
            const int &index,
            const int &Year,
            const int &Nage,
            const std::string &species,
            boost::numeric::ublas::matrix<double> &WtAtAge);

    void nmfQueryMsvpaPreyList(
            std::string selectedSpeciesAgeSizeClass,
            std::string selectedSpecies,
            std::string MSVPAName,
            std::string &PredAge,
            int &NPrey,
            QStringList &PreyList,
            bool includedPredAge);

    void nmfQueryForecastPreyList(
            const std::string &MSVPAName,
            const std::string &ForecastName,
            const std::string &ScenarioName,
            const std::string &Species,
            const bool includePredAge,
            const std::string &PredAge,
            const std::string &seasonStr,
            QStringList &PreyList);

    void nmfQueryNaturalMortalityData(
            const int &SpeciesIndex,
            const int &SSVPAIndex,
            const int &MaxCatchAge,
            const int &NCatchYears,
            boost::numeric::ublas::matrix<double> &M);

    void nmfQueryMaturityData(
            const int &i,
            const int &Year,
            const int &Nage,
            const std::string &species,
            boost::numeric::ublas::matrix<double> &Pmature);

    void nmfQueryMortalityData(
            const std::string &MSVPAName,
            const int &index,
            const int &Year,
            const int &Nage,
            const std::string &species,
            boost::numeric::ublas::matrix<double> &FatAge,
            boost::numeric::ublas::matrix<double> &M1atAge,
            boost::numeric::ublas::matrix<double> &M2atAge);

    bool getMortalityData(
            nmfLogger* logger,
            const std::string& ProjectSettingsConfig,
            const std::string &Species,
            const int &NumYears,
            const int &NumAges,
            const std::string TableName,
            boost::numeric::ublas::matrix<double> &TableData);

    void nmfQueryAgeCatchData(
            const int &i,
            const int &Year,
            const int &Nage,
            const std::string &species,
            boost::numeric::ublas::matrix<double> &Catch);

    void nmfQueryCatchFields(
            const std::string &table, const int &speciesIndex,
            const std::string FirstCatchAgeStr,	const std::string LastCatchAgeStr,
            const std::string OrderByStr, boost::numeric::ublas::matrix<double> &Catch);

    void nmfQueryAlphaBetaFields(
            const std::string &table,
            const int &speciesIndex,
            const std::string &SSVPAName,
            const int &NIndex,
            boost::numeric::ublas::vector<double> &Alpha,
            boost::numeric::ublas::vector<double> &Beta);

    void getSpeciesToIndexMap(std::map<std::string,int>& SpeciesToIndexMap);

    void createScenarioMap(std::map<QString,QStringList>& ScenarioForecastMap);

    bool databaseExists(std::string dbName);

    bool getAlgorithmIdentifiers(
            QWidget*     widget,
            nmfLogger*   logger,
            const std::string& ProjectSettingsConfig,
            std::string& Algorithm,
            std::string& Minimizer,
            std::string& ObjectiveCriterion,
            std::string& Scaling,
            std::string& CompetitionForm,
            const bool&  showPopupError);
    bool loadVector(
            const std::vector<std::map<std::string, std::vector<std::string> > >& dataMaps,
            const int& NumRows,
            std::vector<double>& vec,
            nmfLogger* Logger);

    bool loadMatrix(
            const std::vector<std::map<std::string, std::vector<std::string> > >& dataMaps,
            const int& NumRows,
            const int& NumCols,
            boost::numeric::ublas::matrix<double>& mat,
            nmfLogger* Logger);

    bool getGuildData(nmfLogger* Logger,
                      const int& NumGuilds,
                      const int& RunLength,
                      const QStringList& GuildList,
                      std::map<int,std::vector<int> >& GuildSpecies,
                      std::vector<int>& GuildNum,
                      boost::numeric::ublas::matrix<double>& ObservedBiomassByGuilds);

    bool getHarvestData(const std::string& HarvestType,
                        nmfLogger* Logger,
                        const std::string& ProjectSettingsConfig,
                        const int& NumSpecies,
                        const int& RunLength,
                        boost::numeric::ublas::matrix<double>& Catch,
                        boost::numeric::ublas::matrix<double>& Effort,
                        boost::numeric::ublas::matrix<double>& Exploitation,
                        std::vector<double>& Catchability);

    bool getPredationData(const std::string& PredationType,
                          nmfLogger* Logger,
                          std::string ProjectSettingsConfig,
                          const int& NumSpecies,
                          boost::numeric::ublas::matrix<double>& Rho,
                          boost::numeric::ublas::matrix<double>& Handling,
                          std::vector<double>& Exponent);

    bool getCompetitionData(const std::string& CompetitionType,
                            nmfLogger* Logger,
                            const std::string& ProjectSettingsConfig,
                            const int& NumSpecies,
                            const int& NumGuilds,
                            boost::numeric::ublas::matrix<double>& CompetitionAlpha,
                            boost::numeric::ublas::matrix<double>& CompetitionBetaSpecies,
                            boost::numeric::ublas::matrix<double>& CompetitionBetaGuild,
                            boost::numeric::ublas::matrix<double>& CompetitionBetaGuildGuild);

    bool getSpeciesInitialData(int& NumSpecies,
                               QStringList& SpeciesList,
                               std::vector<double>& InitBiomass,
                               std::vector<double>& GrowthRate,
                               std::vector<double>& SpeciesK,
                               nmfLogger*     Logger);

    bool getModelFormData(std::string& GrowthForm,
                          std::string& HarvestForm,
                          std::string& CompetitionForm,
                          std::string& PredationForm,
                          int&         RunLength,
                          int&         InitialYear,
                          nmfLogger*   Logger,
                          std::string  ProjectSettingsConfig);

    bool getRunLengthAndStartYear(
            nmfLogger* logger,
            const std::string& ProjectSettingsConfig,
            int &RunLength,
            int &StartYear);

    QString importDatabase(QWidget*     widget,
                           nmfLogger*   logger,
                           std::string& ProjectDir,
                           std::string& Username,
                           std::string& Password);

    void exportDatabase(QWidget*     widget,
                        std::string& ProjectDir,
                        std::string& Username,
                        std::string& Password,
                        std::string& ProjectDatabase);

    void loadEstimatedVectorParameters(
            nmfLogger*   logger,
            std::string& projectSettingsConfig,
            QComboBox*   cmbox);

    /**
     * @brief getListOfAuthenticatedDatabaseNames - returns list of databases that are appropriate for this application
     * @param authenticatedDatabases - the list of authenticated databases
     * @return true if found at least one database else false
     */
    bool getListOfAuthenticatedDatabaseNames(
            QList<QString>& authenticatedDatabases);

    /**
     * @brief getAllTables - finds all of the table names in the current database
     * @param databaseTables - the list of table names in current database
     * @return true if found at least one table was found
     */
    bool getAllTables(std::vector<std::string>& databaseTables);

    /**
     * @brief authenticateDatabase - checks to see if database to be loaded is compatible with the application
     * @param databaseName - database name to be loaded
     * @return true or false signifying if database to be loaded is compatible with the application
     */
    bool authenticateDatabase(const std::string& databaseName);

    /**
     * @brief saveApplicationTable - Saves a table that identifies the type of database (i.e., MSVPA, MSSPM, MSCAA)
     * @param widget - the calling widget, needed to know where to show popup error dialogs
     * @param logger - logger widget for logging errors and messages
     */
    void saveApplicationTable(
            QWidget*       widget,
            nmfLogger*     logger,
            std::string&   tableName);

    /**
     * @brief Restores the appropriate CSV from from the given table
     * @param TableName - the MySQL table whose CSV file is to be restored
     * @param ProjectDir - the project directory for the current session
     * @param fields - vector of table field names
     * @return none
     */
    void RestoreCSVFile(QString &TableName,
                        std::string &ProjectDir,
                        std::vector<std::string> &fields);


    // The following are all of the create-if-table-doesn't-exist functions.
    void checkForTableAndCreate(QString table);


    // Made the following static for use in a function map
    static void createApplication(QString &table, QString &qcmd);
    static void createForeEnergyDens(QString &table, QString &qcmd);
    static void createForeOutput(QString &table, QString &qcmd);
    static void createForePredGrowth(QString &table, QString &qcmd);
    static void createForePredVonB(QString &table, QString &qcmd);
    static void createForeSRQ(QString &table, QString &qcmd);
    static void createForeSRR(QString &table, QString &qcmd);
    static void createForeSuitPreyBiomass(QString &table, QString &qcmd);
    static void createForecasts(QString &table, QString &qcmd);
    static void createMSVPAEnergyDens(QString &table, QString &qcmd);
    static void createMSVPAOthPrey(QString &table, QString &qcmd);
    static void createMSVPAOthPreyAnn(QString &table, QString &qcmd);
    static void createMSVPASeasBiomass(QString &table, QString &qcmd);
    static void createMSVPASeasInfo(QString &table, QString &qcmd);
    static void createMSVPASizePref(QString &table, QString &qcmd);
    static void createMSVPASpaceO(QString &table, QString &qcmd);
    static void createMSVPAStomCont(QString &table, QString &qcmd);
    static void createMSVPASuitPreyBiomass(QString &table, QString &qcmd);
    static void createMSVPAlist(QString &table, QString &qcmd);
    static void createMSVPAprefs(QString &table, QString &qcmd);
    static void createMSVPAspecies(QString &table, QString &qcmd);
    static void createOthPredSizeData(QString &table, QString &cmd);
    static void createOtherPredBM(QString &table, QString &cmd);
    static void createOtherPredSpecies(QString &table, QString &qcmd);
    static void createSSVPAAgeM(QString &table, QString &qcmd);
    static void createScenarioF(QString &table, QString &qcmd);
    static void createScenarioOthPred(QString &table, QString &qcmd);
    static void createScenarioOthPrey(QString &table, QString &qcmd);
    static void createScenarioRec(QString &table, QString &qcmd);
    static void createScenarios(QString &table, QString &qcmd);
    static void createSpeCatch(QString &table, QString &qcmd);
    static void createSpeMaturity(QString &table, QString &qcmd);
    static void createSpeSSVPA(QString &table, QString &qcmd);
    static void createSpeSize(QString &table, QString &qcmd);
    static void createSpeTuneCatch(QString &table, QString &qcmd);
    static void createSpeTuneEffort(QString &table, QString &qcmd);
    static void createSpeWeight(QString &table, QString &qcmd);
    static void createSpeXSAData(QString &table, QString &qcmd);
    static void createSpeXSAIndices(QString &table, QString &qcmd);
    static void createSpecies(QString &table, QString &qcmd);



};

#endif /* NMFDATABASE_H_ */
