#!/bin/sh
###############################################################################
#
# OCS Inventory NG Unified Unix Agent Setup script
#
# Copyleft 2008 OCS Inventory NG Team
# http://www.ocsinventory-ng.org
###############################################################################
#
# Usage:
#     setup.sh AGENT_BASE_FILENAME SERVER_STATE_DIR [CONFIG_DIR] [VAR_DIR]
#
# AGENT_BASE_FILENAME = Ocsinventory-Agent-version (without .tar.gz)
# SERVER_ADDRESS = 192.168.1.25:80 for example
# CONFIG_DIR = global agent configuration directory (optional, default to 
#              /etc/ocsinnventory-agent)
# VAR_DIR = global agent var dircetory (optional, default to 
#           /var/lib/ocsinventory-agent)

###############################################################################
############################  DO NOT MODIFY BELOW  ############################
###############################################################################

# Set silent install
export PERL_AUTOINSTALL=1
# Tempory directory to use
TMP_DIR="/tmp"
# Where to store setup log file
SETUP_LOG="$TMP_DIR/ocs_agent_setup.log"
# Agent global configuration file
CONFIG_FILE="ocsinventory-agent.cfg"
# Agent global module file
MODULE_FILE="modules.conf"
# Cron task file name and directory
CRON_TASK="ocsinventory-agent"
CRON_DIR="/etc/cron.d"

###############################################################################
# Setup parameters
echo > $SETUP_LOG
OCS_LOCAL_DATE=`date +%Y-%m-%d-%H-%M-%S`
echo "Starting OCS Inventory NG Agent setup on $OCS_LOCAL_DATE" >> $SETUP_LOG
echo -n "from folder " >> $SETUP_LOG
pwd >> $SETUP_LOG
echo "Storing log in file $SETUP_LOG" >> $SETUP_LOG
echo >> $SETUP_LOG
echo "Parsing script parameters <$1> <$2> <$3> <$4>..." >> $SETUP_LOG
# Setup file base name (must be followed by .tar.gz)
if [ -z $1 ]
then
    echo "*** ERROR: missing first mandatory parameter !" >> $SETUP_LOG
    exit 1
else
    BASE_FILENAME="$1"
fi
echo "Using <$BASE_FILENAME.tar.gz> as Agent source file..." >> $SETUP_LOG
# Server state directory
if [ -z $2 ]
then
    echo "*** ERROR: missing second mandatory parameter !" >> $SETUP_LOG
    exit 1
else
    SERVER_STATE_DIR="$2"
fi
echo "Using <$SERVER_STATE_DIR> as server state directory..." >> $SETUP_LOG
# Agent global configuration directory
if [ -z $3 ]
then
    CONFIG_DIR="/etc/ocsinventory-agent"
else
    CONFIG_DIR=$3
fi
echo "Using <$CONFIG_DIR> as Agent global configuration directory..." >> $SETUP_LOG 
# Agent var dir
if [ -z $4 ]
then
    VAR_DIR="/var/lib/ocsinventory-agent"
else
    VAR_DIR=$4
fi
echo "Using <$VAR_DIR> as Agent global state directory..." >> $SETUP_LOG 

###############################################################################
# Agent setup
echo "Moving to directory $TMP_DIR..." >> $SETUP_LOG
cd $TMP_DIR >> $SETUP_LOG 2>&1
if [ $? -ne 0 ]
then
    echo "*** ERROR Unable to cd to specified temp folder !" >> $SETUP_LOG
    exit 1
fi
echo "Gunzip Agent archive $BASE_FILENAME.tar.gz..." >> $SETUP_LOG
gzip -d "$BASE_FILENAME.tar.gz" >> $SETUP_LOG 2>&1
if [ $? -ne 0 ]
then
    echo "*** ERROR Unable to gunzip Agent archive !" >> $SETUP_LOG
    exit 1
fi
echo "Untar Agent archive $BASE_FILENAME.tar..." >> $SETUP_LOG
tar -xf "$BASE_FILENAME.tar" >> $SETUP_LOG 2>&1
if [ $? -ne 0 ]
then
    echo "*** ERROR Unable to untar Agent archive !" >> $SETUP_LOG
    exit 1
fi
echo "Entering Agent sub directory $BASE_FILENAME..." >> $SETUP_LOG
cd "$BASE_FILENAME" >> $SETUP_LOG 2>&1
if [ $? -ne 0 ]
then
    echo "*** ERROR Unable to enter Agent sub directory !" >> $SETUP_LOG
    exit 1
fi
echo "Configuring Agent (perl Makefile.PL)..." >> $SETUP_LOG
perl Makefile.PL >> $SETUP_LOG 2>&1
if [ $? -ne 0 ]
then
    echo "*** ERROR Unable to configure Agent !" >> $SETUP_LOG
    exit 1
fi
echo "Building Agent (make)..." >> $SETUP_LOG
make >> /dev/null 2>>$SETUP_LOG
if [ $? -ne 0 ]
then
    echo "*** ERROR Unable to build Agent !" >> $SETUP_LOG
    exit 1
fi
echo "Testing Agent (make test)..." >> $SETUP_LOG
make test >> /dev/null 2>>$SETUP_LOG
if [ $? -ne 0 ]
then
    echo "*** WARNING Agent test failed !" >> $SETUP_LOG
fi
echo "Installing Agent (make install)..." >> $SETUP_LOG
make install >> /dev/null 2>>$SETUP_LOG
if [ $? -ne 0 ]
then
    echo "*** ERROR Unable to install Agent !" >> $SETUP_LOG
    exit 1
fi
echo "Leaving Agent directory..." >> $SETUP_LOG
cd .. >> $SETUP_LOG 2>&1
echo "Removing Agent directory $BASE_FILENAME..." >> $SETUP_LOG
rm -Rf "$BASE_FILENAME*" >> $SETUP_LOG 2>&1


###############################################################################
# Agent configurationn
echo "Creating configuration directories..." >> $SETUP_LOG
mkdir -p $CONFIG_DIR >> $SETUP_LOG 2>&1
if [ $? -ne 0 ]
then
    echo "*** ERROR Unable to create $CONFIG_DIR directory !" >> $SETUP_LOG
    exit 1
fi
mkdir -p $VAR_DIR/$SERVER_STATE_DIR >> $SETUP_LOG 2>&1
if [ $? -ne 0 ]
then
    echo "*** ERROR Unable to create $VAR_DIR/$SERVER_STATE_DIR directory !" >> $SETUP_LOG
    exit 1
fi
echo "Creating global configuration files..." >> $SETUP_LOG
cp $TMP_DIR/$CONFIG_FILE $CONFIG_DIR/$CONFIG_FILE >> $SETUP_LOG 2>&1
if [ $? -ne 0 ]
then
    echo "*** ERROR Unable to copy $TMP_DIR/$CONFIG_FILE to $CONFIG_DIR/$CONFIG_FILE !" >> $SETUP_LOG
    exit 1
fi
cp $TMP_DIR/$MODULE_FILE $CONFIG_DIR/$MODULE_FILE >> $SETUP_LOG 2>&1
if [ $? -ne 0 ]
then
    echo "*** ERROR Unable to copy $TMP_DIR/$MODULE_FILE to $CONFIG_DIR/$MODULE_FILE !" >> $SETUP_LOG
    exit 1
fi
if [ -d $TMP_DIR/$SERVER_STATE_DIR ]
then
    echo "Copying additional configuration files to server state directory $SERVER_STATE_DIR..." >> $SETUP_LOG
    cp $TMP_DIR/$SERVER_STATE_DIR/* $VAR_DIR/$SERVER_STATE_DIR >> $SETUP_LOG 2>&1
    if [ $? -ne 0 ]
    then
        echo "*** ERROR Unable to copy files from $TMP_DIR/$SERVER_STATE_DIR to $VAR_DIR/$SERVER_STATE_DIR !" >> $SETUP_LOG
        exit 1
    fi
    rm -Rf "$TMP_DIR/$SERVER_STATE_DIR" >> $SETUP_LOG 2>&1
fi
echo "Creating cron task..." >> $SETUP_LOG
if [ -e $CRON_DIR/$CRON_TASK ]
then
    echo "Cron task already exists, skipping..." >> $SETUP_LOG
else
    CRON_MIN=`date +%M`
    echo "# Run OCS Inventory NG Agent" > $CRON_DIR/$CRON_TASK
    echo "$CRON_MIN * * * * root /usr/bin/ocsinventory-agent --lazy > /dev/null 2>&1" >> $CRON_DIR/$CRON_TASK
    if [ $? -ne 0 ]
    then
        echo "*** ERROR Unable to create $CRON_DIR/$CRON_TASK !" >> $SETUP_LOG
        exit 1
    fi
fi

###############################################################################
# Old Agent for Linux settings migration
if [ -e /etc/ocsinventory-client/ocsinv.conf ]
then
    echo "Migrating settings from old OCS Inventory NG Agent for Linux (/etc/ocsinventory-client/)..." >> $SETUP_LOG
    cp -Rf /etc/ocsinventory-client/* $VAR_DIR/$SERVER_STATE_DIR >> $SETUP_LOG 2>&1
    if [ $? -ne 0 ]
    then
        echo "*** ERROR Unable to copy files from /etc/ocsinventor-client/ to $VAR_DIR/$SERVER_STATE_DIR !" >> $SETUP_LOG
        exit 1
    fi
    rm -Rf /etc/ocsinventory-client >> $SETUP_LOG 2>&1
    if [ -e /bin/ocsinv ]
    then
        rm -f /bin/ocsinv >> $SETUP_LOG 2>&1
    fi
    if [ -e /usr/sbin/ocsinventory-client.pl ]
    then
        rm -f /usr/sbin/ocsinventory-client.pl >> $SETUP_LOG 2>&1
    fi
    if [ -e /etc/logrotate.d/ocsinventory-client ]
    then
        rm -f /etc/logrotate.d/ocsinventory-client >> $SETUP_LOG 2>&1
    fi
    if [ -e /etc/cron.d/ocsinventory-client ]
    then
        rm -f /etc/cron.d/ocsinventory-client >> $SETUP_LOG 2>&1
    fi
fi

###############################################################################
# SUCCESS
echo "SUCCESS installing OCS Inventory NG Agent ;-)" >> $SETUP_LOG
exit 0

