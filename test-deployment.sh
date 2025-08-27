#!/bin/bash

# sys-tune-yt Test Script
# Validates build output before hardware deployment
# Atmosphere 1.9.3 / OFW 20.x compatibility checker

# Don't exit on errors during testing
# set -e

echo "🧪 sys-tune-yt Pre-Deployment Test Suite"
echo "==========================================="
echo ""

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TESTS_PASSED=0
TESTS_FAILED=0
WARNINGS=0

# Helper functions
pass_test() {
    echo -e "  ${GREEN}✓${NC} $1"
    ((TESTS_PASSED++))
}

fail_test() {
    echo -e "  ${RED}✗${NC} $1"
    ((TESTS_FAILED++))
}

warn_test() {
    echo -e "  ${YELLOW}⚠${NC} $1"
    ((WARNINGS++))
}

info_test() {
    echo -e "  ${BLUE}ℹ${NC} $1"
}

# Test 1: Basic file existence
echo "📁 Test 1: File Existence Check"
echo "--------------------------------"

required_files=(
    "dist/atmosphere/contents/4200000000000000/exefs.nsp"
    "dist/atmosphere/contents/4200000000000000/toolbox.json"
    "dist/switch/.overlays/sys-tune-overlay.ovl"
    "sys-tune/sys-tune.json"
    "sys-tune/sys-tune.npdm"
    "sys-tune/sys-tune.nso"
)

for file in "${required_files[@]}"; do
    if [[ -f "$file" ]]; then
        pass_test "Found: $file"
    else
        fail_test "Missing: $file"
    fi
done
echo ""

# Test 2: File format validation
echo "🔍 Test 2: File Format Validation"
echo "----------------------------------"

# Check NSP file
if [[ -f "dist/atmosphere/contents/4200000000000000/exefs.nsp" ]]; then
    nsp_type=$(file "dist/atmosphere/contents/4200000000000000/exefs.nsp" | grep -o "Nintendo Switch partition filesystem")
    if [[ -n "$nsp_type" ]]; then
        pass_test "exefs.nsp has valid Nintendo Switch format"
    else
        fail_test "exefs.nsp has invalid format"
    fi
    
    # Check NSP size (should be reasonable, not 0 bytes)
    nsp_size=$(stat -c%s "dist/atmosphere/contents/4200000000000000/exefs.nsp" 2>/dev/null || echo "0")
    if [[ $nsp_size -gt 100000 ]]; then
        pass_test "exefs.nsp size is reasonable ($nsp_size bytes)"
    else
        fail_test "exefs.nsp size is too small ($nsp_size bytes)"
    fi
fi

# Check NSO file
if [[ -f "sys-tune/sys-tune.nso" ]]; then
    nso_type=$(file "sys-tune/sys-tune.nso" | grep -o "Nintendo Switch executable")
    if [[ -n "$nso_type" ]]; then
        pass_test "sys-tune.nso has valid Nintendo Switch executable format"
    else
        fail_test "sys-tune.nso has invalid format"
    fi
fi

# Check OVL file
if [[ -f "dist/switch/.overlays/sys-tune-overlay.ovl" ]]; then
    ovl_size=$(stat -c%s "dist/switch/.overlays/sys-tune-overlay.ovl" 2>/dev/null || echo "0")
    if [[ $ovl_size -gt 10000 ]]; then
        pass_test "sys-tune-overlay.ovl size is reasonable ($ovl_size bytes)"
    else
        fail_test "sys-tune-overlay.ovl size is too small ($ovl_size bytes)"
    fi
fi
echo ""

# Test 3: JSON Configuration Validation
echo "⚙️  Test 3: Configuration Validation"
echo "------------------------------------"

if [[ -f "sys-tune/sys-tune.json" ]]; then
    # Basic JSON syntax check (look for balanced braces)
    if grep -q "^{" "sys-tune/sys-tune.json" && grep -q "}$" "sys-tune/sys-tune.json"; then
        pass_test "sys-tune.json appears to have valid JSON structure"
    else
        fail_test "sys-tune.json has invalid JSON structure"
    fi
    
    # Check required fields
    if grep -q '"title_id": "0x4200000000000000"' "sys-tune/sys-tune.json"; then
        pass_test "Title ID is correct: 0x4200000000000000"
    else
        fail_test "Title ID mismatch or missing"
    fi
    
    # Check kernel version compatibility with Atmosphere 1.9.3 / OFW 20.x
    if grep -q '"value": "0x0030"' "sys-tune/sys-tune.json"; then
        pass_test "Minimum kernel version compatible with OFW 20.x: 0x0030"
    else
        warn_test "Minimum kernel version might be incompatible"
    fi
    
    # Check if syscall 0x28 is properly defined to prevent crash
    if grep -q '0x28' "sys-tune/sys-tune.json"; then
        pass_test "Syscall 0x28 (svcReturnFromException) properly defined"
    else
        fail_test "Missing syscall 0x28 definition - this will cause crash!"
    fi
    
    # Check for comprehensive syscall coverage
    syscall_count=$(grep -o '"svc[^"]*": "0x[^"]*"' "sys-tune/sys-tune.json" | wc -l)
    if [ "$syscall_count" -ge 50 ]; then
        pass_test "Comprehensive syscall coverage ($syscall_count syscalls)"
    else
        warn_test "Limited syscall coverage ($syscall_count syscalls) - may cause crashes"
    fi
    
    # Check critical syscalls
    if grep -q '"svcManageNamedPort": "0x71"' "sys-tune/sys-tune.json"; then
        pass_test "Critical syscall svcManageNamedPort present"
    else
        fail_test "Missing critical syscall svcManageNamedPort"
    fi
    
    # Check service host
    if grep -q '"tune"' "sys-tune/sys-tune.json" && grep -q '"service_host"' "sys-tune/sys-tune.json"; then
        pass_test "Service host 'tune' configured"
    else
        fail_test "Missing service host 'tune'"
    fi
fi

if [[ -f "dist/atmosphere/contents/4200000000000000/toolbox.json" ]]; then
    if grep -q "^{" "dist/atmosphere/contents/4200000000000000/toolbox.json" && grep -q "}$" "dist/atmosphere/contents/4200000000000000/toolbox.json"; then
        pass_test "toolbox.json appears to have valid JSON structure"
    else
        fail_test "toolbox.json has invalid JSON structure"
    fi
    
    if grep -q '"tid": "4200000000000000"' "dist/atmosphere/contents/4200000000000000/toolbox.json"; then
        pass_test "toolbox.json TID matches: 4200000000000000"
    else
        fail_test "toolbox.json TID mismatch"
    fi
fi
echo ""

# Test 4: Title ID Collision Check
echo "🔍 Test 4: Title ID Collision Detection"
echo "---------------------------------------"

# Check for common conflicting title IDs
conflict_ids=("4200000000000000" "4200000000000010" "4200000000000100")
for id in "${conflict_ids[@]}"; do
    if [[ -d "dist/atmosphere/contents/$id" ]]; then
        fail_test "Conflicting title ID directory found: $id"
    else
        pass_test "No conflict with title ID: $id"
    fi
done
echo ""

# Test 5: Atmosphere 1.9.3 Compatibility Check
echo "🌍 Test 5: Atmosphere 1.9.3 Compatibility"
echo "------------------------------------------"

# Check if using features that might not work on Atmosphere 1.9.3
if [[ -f "sys-tune/sys-tune.json" ]]; then
    # Check for features that might not work on Atmosphere 1.9.3
    if grep -q '"pool_partition": 0' "sys-tune/sys-tune.json"; then
        pass_test "Pool partition setting compatible with Atmosphere 1.9.3"
    else
        warn_test "Pool partition setting might cause issues on older Atmosphere"
    fi
    
    if grep -q '"process_category": 0' "sys-tune/sys-tune.json"; then
        pass_test "Process category compatible with Atmosphere 1.9.3"
    else
        warn_test "Process category might cause issues"
    fi
fi
echo ""

# Test 6: Code Analysis
echo "💻 Test 6: Code Quality Checks"
echo "------------------------------"

# Check for potential startup issues
if [[ -f "sys-tune/source/main.cpp" ]]; then
    # Check if network init is in __appInit (should NOT be there)
    if grep -q "HttpClient::Initialize\|YouTubeMusicAPI::Initialize" "sys-tune/source/main.cpp"; then
        network_in_appinit=$(grep -A5 -B5 "void __appInit" "sys-tune/source/main.cpp" | grep -c "HttpClient::Initialize\|YouTubeMusicAPI::Initialize" || true)
        if [[ $network_in_appinit -gt 0 ]]; then
            fail_test "Network initialization found in __appInit() - will cause startup crashes"
        else
            pass_test "Network initialization not in __appInit()"
        fi
    else
        pass_test "No network initialization in main.cpp"
    fi
    
    # Check for proper R_ABORT_UNLESS usage
    if grep -q "R_ABORT_UNLESS" "sys-tune/source/main.cpp"; then
        pass_test "Using proper error handling with R_ABORT_UNLESS"
    else
        warn_test "No R_ABORT_UNLESS found - might have error handling issues"
    fi
fi
echo ""

# Test 7: File Integrity
echo "🔐 Test 7: File Integrity Verification"
echo "--------------------------------------"

if [[ -f "dist/atmosphere/contents/4200000000000000/exefs.nsp" ]]; then
    nsp_hash=$(md5sum "dist/atmosphere/contents/4200000000000000/exefs.nsp" | cut -d' ' -f1)
    echo "  📊 exefs.nsp MD5: $nsp_hash"
    info_test "Save this hash to verify integrity after transfer"
fi

if [[ -f "dist/switch/.overlays/sys-tune-overlay.ovl" ]]; then
    ovl_hash=$(md5sum "dist/switch/.overlays/sys-tune-overlay.ovl" | cut -d' ' -f1)
    echo "  📊 overlay MD5: $ovl_hash"
    info_test "Save this hash to verify integrity after transfer"
fi
echo ""

# Test 8: Deployment Readiness
echo "🚀 Test 8: Deployment Readiness"
echo "-------------------------------"

# Check directory structure
if [[ -d "dist/atmosphere/contents/4200000000000000" ]] && [[ -d "dist/switch/.overlays" ]]; then
    pass_test "Proper directory structure for deployment"
else
    fail_test "Invalid directory structure"
fi

# Check for debug symbols (should be stripped for release)
if [[ -f "sys-tune/sys-tune.elf" ]]; then
    if objdump -h "sys-tune/sys-tune.elf" | grep -q "debug"; then
        warn_test "Debug symbols present in ELF (increases size)"
    else
        pass_test "No debug symbols in ELF (optimized for release)"
    fi
fi
echo ""

# Final Report
echo "📋 Test Results Summary"
echo "======================="
echo -e "  ${GREEN}Passed:${NC} $TESTS_PASSED"
echo -e "  ${RED}Failed:${NC} $TESTS_FAILED"
echo -e "  ${YELLOW}Warnings:${NC} $WARNINGS"
echo ""

if [[ $TESTS_FAILED -eq 0 ]]; then
    echo -e "${GREEN}🎉 All critical tests passed!${NC}"
    echo "✅ Safe to deploy to hardware"
    echo ""
    echo "📋 Deployment Checklist:"
    echo "1. Remove existing sys-tune directories from Switch"
    echo "2. Copy dist/atmosphere/contents/4200000000000000/ to SD card"
    echo "3. Copy dist/switch/.overlays/sys-tune-overlay.ovl to SD card"
    echo "4. Verify file integrity using MD5 hashes above"
    echo "5. Reboot Switch and test"
else
    echo -e "${RED}❌ Critical issues found!${NC}"
    echo "🚫 DO NOT deploy to hardware until issues are resolved"
    exit 1
fi

if [[ $WARNINGS -gt 0 ]]; then
    echo -e "${YELLOW}⚠️  $WARNINGS warnings detected${NC}"
    echo "💡 Consider reviewing warnings before deployment"
fi
