#! /usr/bin/env perl
# Copyright 2000-2018 The OpenSSL Project Authors. All Rights Reserved.
#
# Licensed under the Apache License 2.0 (the "License").  You may not use
# this file except in compliance with the License.  You can obtain a copy
# in the file LICENSE in the source distribution or at
# https://www.openssl.org/source/license.html

use strict;

my ($i, @arr);

# Set up an array with the type of ASCII characters
# Each set bit represents a character property.

# RFC2253 character properties
my $RFC2253_ESC = 1;	# Character escaped with \
my $ESC_CTRL	= 2;	# Escaped control character
# These are used with RFC1779 quoting using "
my $NOESC_QUOTE	= 8;	# Not escaped if quoted
my $PSTRING_CHAR = 0x10;	# Valid PrintableString character
my $RFC2253_FIRST_ESC = 0x20; # Escaped with \ if first character
my $RFC2253_LAST_ESC = 0x40;  # Escaped with \ if last character
my $RFC2254_ESC = 0x400;	# Character escaped \XX
my $HOST_ANY = 0x1000;      # Valid hostname character anywhere in label
my $HOST_DOT = 0x2000;  # Dot: hostname label separator
my $HOST_HYPHEN = 0x4000; # Hyphen: not valid at start or end.
my $HOST_WILD = 0x8000; # Wildcard character

for($i = 0; $i < 128; $i++) {
	# Set the RFC2253 escape characters (control)
	$arr[$i] = 0;
	if(($i < 32) || ($i > 126)) {
		$arr[$i] |= $ESC_CTRL;
	}

	# Some PrintableString characters
	if(		   ( ( $i >= ord("a")) && ( $i <= ord("z")) )
			|| (  ( $i >= ord("A")) && ( $i <= ord("Z")) )
			|| (  ( $i >= ord("0")) && ( $i <= ord("9")) )  ) {
		$arr[$i] |= $PSTRING_CHAR | $HOST_ANY;
	}
}

# Now setup the rest

# Remaining RFC2253 escaped characters

$arr[ord(" ")] |= $NOESC_QUOTE | $RFC2253_FIRST_ESC | $RFC2253_LAST_ESC;
$arr[ord("#")] |= $NOESC_QUOTE | $RFC2253_FIRST_ESC;

$arr[ord(",")] |= $NOESC_QUOTE | $RFC2253_ESC;
$arr[ord("+")] |= $NOESC_QUOTE | $RFC2253_ESC;
$arr[ord("\"")] |= $RFC2253_ESC;
$arr[ord("\\")] |= $RFC2253_ESC;
$arr[ord("<")] |= $NOESC_QUOTE | $RFC2253_ESC;
$arr[ord(">")] |= $NOESC_QUOTE | $RFC2253_ESC;
$arr[ord(";")] |= $NOESC_QUOTE | $RFC2253_ESC;

# Remaining RFC2254 characters

$arr[0] |= $RFC2254_ESC;
$arr[ord("(")] |= $RFC2254_ESC;
$arr[ord(")")] |= $RFC2254_ESC;
$arr[ord("*")] |= $RFC2254_ESC | $HOST_WILD;
$arr[ord("\\")] |= $RFC2254_ESC;

# Remaining PrintableString characters

$arr[ord(" ")] |= $PSTRING_CHAR;
$arr[ord("'")] |= $PSTRING_CHAR;
$arr[ord("(")] |= $PSTRING_CHAR;
$arr[ord(")")] |= $PSTRING_CHAR;
$arr[ord("+")] |= $PSTRING_CHAR;
$arr[ord(",")] |= $PSTRING_CHAR;
$arr[ord("-")] |= $PSTRING_CHAR | $HOST_HYPHEN;
$arr[ord(".")] |= $PSTRING_CHAR | $HOST_DOT;
$arr[ord("/")] |= $PSTRING_CHAR;
$arr[ord(":")] |= $PSTRING_CHAR;
$arr[ord("=")] |= $PSTRING_CHAR;
$arr[ord("?")] |= $PSTRING_CHAR;

# Now generate the C code

# Output year depends on the year of the script.
my $YEAR = [localtime([stat($0)]->[9])]->[5] + 1900;
print <<EOF;
/*
 * WARNING: do not edit!
 * Generated by crypto/asn1/charmap.pl
 *
 * Copyright 2000-$YEAR The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#define CHARTYPE_HOST_ANY $HOST_ANY
#define CHARTYPE_HOST_DOT $HOST_DOT
#define CHARTYPE_HOST_HYPHEN $HOST_HYPHEN
#define CHARTYPE_HOST_WILD $HOST_WILD

/*
 * Mask of various character properties
 */

static const unsigned short char_type[] = {
EOF

print "   ";
for($i = 0; $i < 128; $i++) {
	print("\n   ") if($i && (($i % 12) == 0));
	printf(" %4d", $arr[$i]);
	print(",") if ($i != 127);
}
print("\n};\n");
