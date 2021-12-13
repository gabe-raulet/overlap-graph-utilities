#!/bin/bash

SEQUENCES=$1
THREADS=$2

minimap2 -X -x ava-pb -t $THREADS $SEQUENCES $SEQUENCES
