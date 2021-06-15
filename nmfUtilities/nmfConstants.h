/**
 * @file nmfConstants.h
 * @brief Definition for common constants used in MultiSpecies tools
 * @date Dec 8, 2016
 *
 * This file contains constant declarations used in the MultiSpecies tools using
 * the nmfConstants namespace.
 */

#ifndef NMFCONSTANTS_H
#define NMFCONSTANTS_H

#include <iostream>
#include <vector>

#include <math.h>


namespace nmfConstants
{

    const bool   nullsAsEmptyString = true;
    const bool   nullAsZero = false;
    const bool   RoundOff = true;
    const bool   NoSignal = false;
    const bool   RotateLabels = true;
    const bool   DontRotateLabels = false;

    const int    TYPE_PREDATOR       = 0;
    const int    TYPE_PREY           = 1;
    const int    TYPE_OTHER_PREDATOR = 3;

    const int    MaxNumberLogFiles   =  50;
    const int    DefaultReferenceAge =   2;
    const int    MinSliderValue      =   0;
    const int    MaxSliderValue      =  99;
    const int    MaxNumberSeasons    =   6;
    const int    MaxNumberYears      = 100;
    const int    MaxNumberAges       =  21;
    const int    MaxNumberSpecies    =  12;
    const int    MaxNumberOtherPredators =   12;
    const int    MaxNumberPrey           =   20;
    const int    MaxNumberFleets         =    6;
    const int    MaxNumberIndices        =  100;
    const int    MaxNumberSizeCats       =   20;
    const int    MaxTotalSpecies         =   50;
    const int    NumCategoriesForVerticalNotation = 10;
    const int    NumFeedingDataRows      =  7;
    const int    MinNumForecastYears     =  3;
    const int    MaxNumForecastYears     = 20;
    const int    MaxPathLength           = 1000;
    const bool   RearrangeTitle          = true;
    const bool   DontRearrangeTitle      = false;
    const bool   Clear                   = true;
    const bool   NoClear                 = false;
    const bool   ShowError               = true;
    const bool   DontShowError           = false;
    const bool   IsProportion            = true;
    const bool   IsNotProportion         = false;
    const bool   includeTotalColumn      = true;
    const bool   dontIncludeTotalColumn  = false;
    const bool   autoFillSegments        = true;
    const bool   dontAutoFillSegments    = true;
    const bool   FirstYearOnly           = true;
    const bool   AllYears                = false;
    const bool   Checked                 = true;
    const bool   NotChecked              = false;
    const bool   ShowShadow              = true;
    const bool   DontShowShadow          = false;
    const bool   ReverseAxis             = true;
    const bool   DontReverseAxis         = false;
    const bool   ShowLegend              = true;
    const bool   DontShowLegend          = false;
    const bool   TimestampWithUnderscore = true;
    const bool   TimestampWithoutUnderscore = false;
    const bool   ManuallySetYMinMax      = true;
    const bool   DontManuallySetYMinMax  = false;

    const double KgToLbs       = 2.20462;
    const double MortalityMax  = 5.0;
    const double MortalityInit = 1.0;
    const double MortalityMin  = 0.0;
    const double MortalityInc  = 0.01;
    const double TerminalMortalityMin = 0.01;
    const double TerminalMortalityMax = 5.0;
    const double DefaultTerminalMortality = 0.5;
    const double DefaultResidualNaturalMortality = 0.25;
    const double DefaultPredationMortality = 0.0;
    const double SelectivityMin  = 0.0;
    const double SelectivityMax  = 1.0;
    const double MaturityMin     = 0.0;
    const double MaturityMax     = 1.0;
    const int    NoValueInt      = -9999;
    const double NoValueDouble   = -9999.0;
    const int    ProgressUpdateFrequency = 1000; // How often to update progress bar (milliseconds).
    const double MaxBiomass      = 1e12;
    const double NearlyZero      = 0.000001;
    const double RAD_to_DEG      = (180.0/M_PI);

    const int    Normal    = 0;
    const int    Bold      = 1;
    const int    Warning   = 2;
    const int    Error     = 3;
    const int    Highlight = 4;
    const int    Section   = 5;

    const std::string  DeepSkyBlue = "#00BFFF";
    const std::string  Blue        = "#0072B2";
    const std::string  Orange      = "#E69F00";
    const std::string  Black       = "#000000";
    const std::string  SkyBlue     = "#56B4E9";
    const std::string  Gray        = "#C8C8C8";
    const std::string  Red         = "#FF0000";
    const std::vector<std::string> LineColors     = {Blue,Orange,Black,SkyBlue,Gray,Red};
    const std::vector<std::string> LineColorNames = {"Blue","Orange","Black","Sky Blue","Gray","Red"};

    const std::string  ClearWindow = "";
    const std::string  BlankLine = " ";

    const std::vector<std::string> SurfaceThemes = {
            "Qt",
            "Primary Colors",
            "Digia",
            "Stone Moss",
            "Army Blue",
            "Retro",
            "Ebony",
            "Isabelle"
    };
}

#endif

