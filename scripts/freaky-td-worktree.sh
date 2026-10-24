#!/usr/bin/env bash

FREAKY_TD_URL=https://github.com/hugoschool/freaky-td
FREAKY_TD_BRANCH=main
FREAKY_TD_FOLDER=freaky-td

if [ ! -d $FREAKY_TD_FOLDER ]; then
    git subtree add --prefix $FREAKY_TD_FOLDER $FREAKY_TD_URL $FREAKY_TD_BRANCH --message "chore: import freaky-td"
fi

git subtree pull --prefix $FREAKY_TD_FOLDER $FREAKY_TD_URL $FREAKY_TD_BRANCH --message "chore: update freaky-td from main repo"
