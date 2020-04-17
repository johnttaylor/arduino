/*-----------------------------------------------------------------------------
* This file is part of the Arduino Project.  The Arduino Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/arduino/license.txt
*
* Copyright (c) 2017 John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/


#include "Battery.h"
#include "Cpl/System/Trace.h"
#include "Bsp/Api.h"
#include "Cpl/Text/Tokenizer/TextBlock.h"

#define SECT_   "cmd::battery"

/// Namespaces
using namespace Golem::TShell::Cmd;

#ifndef OPTION_BSP_NUM_BATTERY_ADC_SAMPLES
#define OPTION_BSP_NUM_BATTERY_ADC_SAMPLES      5
#endif



///////////////////////////
Battery::Battery( Cpl::Container::Map<Cpl::TShell::Command>& commandList ) noexcept
    : Cpl::TShell::Cmd::Command( commandList, "battery" )
{
}


/////////////////////////////////////////////////////////
Cpl::TShell::Command::Result_T Battery::execute( Cpl::TShell::Context_& context, char* rawCmdString, Cpl::Io::Output& outfd ) noexcept
{
    Cpl::Text::Tokenizer::TextBlock tokens( rawCmdString, context.getDelimiterChar(), context.getTerminatorChar(), context.getQuoteChar(), context.getEscapeChar() );
    Cpl::Text::String&              outtext    = context.getOutputBuffer();
    bool                            io         = true;
    unsigned                        numParms   = tokens.numParameters();

    // Read/Get battery status
    if ( numParms == 1 )
    {
        float   sumBattMv = 0;
        float   sumAdcMv  = 0;
        int     sumRawAdc = 0;
        int     i;

        for ( i=0; i < OPTION_BSP_NUM_BATTERY_ADC_SAMPLES; i++ )
        {
            float   battMv;
            float   adcMv;
            int     rawAdc;

            Bsp_readBatteryAdc( &battMv, 0, &adcMv, &rawAdc );

            sumBattMv += battMv;
            sumAdcMv  += adcMv;
            sumRawAdc += rawAdc;
        }

        // Calculate average values
        sumBattMv /= OPTION_BSP_NUM_BATTERY_ADC_SAMPLES;
        sumAdcMv  /= OPTION_BSP_NUM_BATTERY_ADC_SAMPLES;
        sumRawAdc /= OPTION_BSP_NUM_BATTERY_ADC_SAMPLES;

        // Output results
        uint8_t battPercent = Bsp_batteryAdcMvToPercent( sumAdcMv );
        outtext.format( "battery: remaining=%d%%, voltage=%dmv, rawAdc=%d, adc=%dmv", battPercent, (int) (sumBattMv + 0.5), sumRawAdc, (int) (sumAdcMv + 0.5) );
        io &= context.writeFrame( outtext );
        return io ? Command::eSUCCESS : Command::eERROR_IO;
    }
    return eERROR_INVALID_ARGS;
}


