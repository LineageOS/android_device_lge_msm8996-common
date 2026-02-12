#!/bin/bash
#
# extract-all.sh
#
# Orchestrates proprietary blob extraction for LG msm8996-based devices.
#
# This script:
#   - Extracts kanged common blobs from external reference devices (daisy, OP3)
#   - Extracts stock common blobs for msm8996-common
#   - Extracts device-common blobs via representative device trees
#     (v20 via us996, g6 via us997, g5 via h850)
#   - Extracts target-specific blobs for all supported device trees
#

set -euo pipefail

## Dump Paths
## You will need to populat these with your dump paths
# alice (g5)
ALICE_N_H850="/mnt/storage/dumps/lge/alice-n-h850"
ALICE_O_H830="/mnt/storage/dumps/lge/alice-o-h830"
ALICE_O_H850="/mnt/storage/dumps/lge/alice-o-h850"

# elsa (v20)
ELSA_N_US996="/mnt/storage/dumps/lge/elsa-n-us996"
ELSA_O_H918="/mnt/storage/dumps/lge/elsa-o-h918"
ELSA_O_H990="/mnt/storage/dumps/lge/elsa-o-h990"
ELSA_O_US996="/mnt/storage/dumps/lge/elsa-o-us996"
ELSA_O_VS995="/mnt/storage/dumps/lge/elsa-o-vs995"
ELSA_P_KOR="/mnt/storage/dumps/lge/elsa-p-kor"

# lucye (g6)
LUCYE_O_H870="/mnt/storage/dumps/lge/lucye-o-h870"
LUCYE_O_H872="/mnt/storage/dumps/lge/lucye-o-h872"
LUCYE_O_US997="/mnt/storage/dumps/lge/lucye-o-us997"
LUCYE_P_KOR="/mnt/storage/dumps/lge/lucye-p-kor"

# daisy/oneplus3 (Kanged Files)
DAISY_Q="/mnt/storage/dumps/xiaomi/daisy/dump"
OP3_P="/mnt/storage/dumps/oneplus/oneplus3-msm8996"

declare -A TARGETS=(
  [h830]="$ALICE_O_H830"
  [h850]="$ALICE_O_H850"
  [h870]="$LUCYE_O_H870"
  [h872]="$LUCYE_O_H872"
  [h918]="$ELSA_O_H918"
  [h990]="$ELSA_O_H990"
  [us996]="$ELSA_O_US996"
  [us996d]="$ELSA_O_US996"
  [us997]="$LUCYE_O_US997"
  [vs995]="$ELSA_O_VS995"
)

### Kangs
## daisy - msm8953
for SECTION_HEADER in \
  "CNE - from daisy - QKQ1.191002.002" \
  "DPM - from daisy - QKQ1.191002.002" \
  "GPS - from daisy - QKQ1.191002.002" \
  "Graphics (Adreno) - from daisy - QKQ1.191002.002" \
  "Graphics (HDR) - from daisy - QKQ1.191002.002" \
  "Graphics (SDM) - from daisy - QKQ1.191002.002" \
  "Graphics (Vulkan) - from daisy - QKQ1.191002.002" \
  "IMS - from daisy - QKQ1.191002.002" \
  "Media - from daisy - QKQ1.191002.002" \
  "Peripheral manager - from daisy - QKQ1.191002.002" \
  "QMI - from daisy - QKQ1.191002.002" \
  "RIL - from daisy - QKQ1.191002.002" \
  "Time services - from daisy - QKQ1.191002.002" \
  "Widevine - from daisy - QKQ1.191002.002"
do
  # Use us997 as the reference device
  ( cd ../us997 && ./extract-files.sh --only-extract --only-common -k -s "${SECTION_HEADER}" "${DAISY_Q}" )
done

## op3 - msm8996
for SECTION_HEADER in \
  "Perf - from op3 - PKQ1.181203.001" \
  "PostprocessColor - from op3 - PKQ1.181203.001" \
  "Thermal - from op3 - PKQ1.181203.001"
do
  # Use us997 as the reference device
  ( cd ../us997 && ./extract-files.sh --only-extract --only-common -k -s "${SECTION_HEADER}" "${OP3_P}" )
done

### Stock
## Extract for common tree
# Use us997 as the reference device
( cd ../us997 && ./extract-files.sh --only-common "${ELSA_O_US996}" )

## Extract for all device-common trees
( cd ../us996 && ./extract-files.sh --only-extract --only-device-common -k -s "Fingerprint - from elsa_nao_us-user 7.0 NRD90M 180331701b134 release-keys" "${ELSA_O_US996}" )
( cd ../us996 && ./extract-files.sh --only-device-common "${ELSA_P_KOR}" )

( cd ../us997 && ./extract-files.sh --only-extract --only-device-common -k -s "Fingerprint - from lucye_nao_us-user 8.0.0 OPR1.170623.032 190420940e75c release-keys" "${LUCYE_O_US997}" )
( cd ../us997 && ./extract-files.sh --only-device-common "${LUCYE_P_KOR}" )

( cd ../h850 && ./extract-files.sh --only-device-common -k -s "Fingerprint - from h1_global_com-user 7.0 NRD90U 181350252b2c6 release-keys" "${ALICE_N_H850}" )
( cd ../h850 && ./extract-files.sh --only-extract --only-device-common "${ALICE_O_H850}" )

## Extract for all target trees (stable order)
for dir in h830 h850 h870 h872 h918 h990 us996 us996d us997 vs995; do
  ( cd "../$dir" && ./extract-files.sh --only-target "${TARGETS[$dir]}" )
done
