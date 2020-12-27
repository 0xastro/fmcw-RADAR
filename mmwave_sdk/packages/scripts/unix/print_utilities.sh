#!/bin/bash
#
# ******************************************************************
# * FILE PURPOSE: print utilities
# ******************************************************************
# * FILE NAME: print_utilities.sh
# *
# * DESCRIPTION:
# *  Print utiltities.
# *
# ******************************************************************

########################################################################################
# Utility function which is used to print an error message on the build console
########################################################################################
function printErrorMsg ()
{
  echo -e "\033[31m"${1}${2}
  echo -e -n "\033[0m"
}

########################################################################################
# Utility function which is used to print a successful message on the build console
########################################################################################
function printSuccessMsg ()
{
  echo -e "\033[32m"${1}${2}
  echo -e -n "\033[0m"
}

########################################################################################
# Utility function which is used to print an informational message on the build console
########################################################################################
function printInfoMsg ()
{
  echo -e "\033[33m"${1}${2}
  echo -e -n "\033[0m"
}

