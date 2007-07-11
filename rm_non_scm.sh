#!/bin/sh
rm -rf `svn stat | grep ^? | sed s/.......//`
