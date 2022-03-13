// https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-instruction-set-reference-manual-325383.pdf

/*
   Initial EAX Value:
     0H - 03H:   Basic CPUID Information
     04H:        Deterministic Cache Paramters Leaf
     05H:        MONITOR/MWAIT Leaf
     06H:        Thermal and Power Management Leaf
     07H:        Structured Extended Feature Flags Enumeration Leaf (Output depends on ECX input value)
     09H:        Direct Cache Access Information Leaf
     0AH:        Architectural Performance Monitoring Leaf
     0BH:        Extended Topology Enumeration Leaf
     0DH:        Processor Extended State Enumeration Main Leaf (EAX = 0DH, ECX = 0)
                 Processor Extended State Enumeration Sub-leaf (EAX = 0DH, ECX = 1)
                 Processor Extended State Enumeration Sub-leaves (EAX = 0DH, ECX = n, n > 1)
     0FH:        Intel Resource Director Technology (Intel RDT) Monitoring Enumeration Sub-leaf (EAX = 0FH, ECX = 0)
                 L3 Cache Intel RDT Monitoring Capability Enumeration Sub-leaf (EAX = 0FH, ECX = 1)
     10H:        Intel Resource Director Technology (Intel RDT) Allocation Enumeration Sub-leaf (EAX = 10H, ECX = 0)
                 L3 Cache Allocation Technology Enumeration Sub-leaf (EAX = 10H, ECX = ResID = 1)
                 L2 Cache Allocation Technology Enumeration Sub-leaf (EAX = 10H, ECX = ResID = 2)
     12H:        Intel SGX Capability Enumeration Leaf, sub-leaf 0 (EAX = 12H, ECX = 0)
                 Intel SGX Attributes Enumeration Leaf, sub-leaf 1 (EAX = 12H, ECX = 1)
                 Intel SGX EPC Enumeration Leaf, sub-leaves (EAX = 12H, ECX = 2 or higher)
     14H:        Intel Processor Trace Enumeration Main Leaf (EAX = 14H, ECX = 0)
                 Intel Processor Trace Enumeration Sub-leaf (EAX = 14H, ECX = 1)
     15H:        Time Stamp Counter and Nominal Core Crystal Clock Information Leaf
     16H:        Processor Frequency Information Leaf
     17H:        System-On-Chip Vendor Attribute Enumeration Main Leaf (EAX = 17H, ECX = 0)
                 System-On-Chip Vendor Attribute Enumeration Sub-leaf (EAX = 17H, ECX = 1..3)
                 System-On-Chip Vendor Attribute Enumeration Sub-leaves (EAX = 17H, ECX > MaxSOCID_Index)
     40000000H
         -    :  Unimplemented CPUID Leaf Functions
     4fffffffH

     80000000H
     80000001H
     80000002H
     80000003H
     80000004H : Extended CPUID Information
     80000005H
     80000006H
     80000007H
     80000008H
*/

#ifndef CPUID_H
#define CPUID_H

void cpuid_get_cpuid(int eax, int *cpuinfo);
void cpuid_get_cpu_vendor_string(int *cpuinfo, char *str);

// EAX = 0H

// EAX
#define CPUID_HIGHEST_VALUE_FOR_BASIC_PROCESSOR_INFORMATION 0xff

// EAX = 01H

// EAX
#define CPUID_STEPPING_ID         0x3
#define CPUID_MODEL               0xf0
#define CPUID_FAMILY              0xf00
#define CPUID_PROCESSOR_TYPE      0x3000
// Bits 14-15 reserved.
#define CPUID_EXTENDED_MODEL_ID   0x70000
#define CPUID_EXTENDED_FAMILY_ID  0x3f80000
// Bits 28-31 reserved.

// EBX
#define CPUID_BRAND_INDEX       0xff
#define CPUID_CLFLUSH_LINE_SIZE 0xff00
#define CPUID_MAX_NUMBER_OF_ADDRESSABLE_IDS_FOR_LOGICAL_PROCESSORS 0xff0000
#define CPUID_INITIAL_APIC_ID   0xff000000

// ECX
#define CPUID_SSE3_IS_SUPPORTED             0x1
#define CPUID_PCLMULQDQ_IS_SUPPORTED        0x2
#define CPUID_DTES64_IS_SUPPORTED           0x4
#define CPUID_MONITOR_IS_SUPPORTED          0x8
#define CPUID_DS_CPL_IS_SUPPORTED           0x10
#define CPUID_VMX_IS_SUPPORTED              0x20
#define CPUID_SMX_IS_SUPPORTED              0x40
#define CPUID_EIST_IS_SUPPORTED             0x80
#define CPUID_TM2_IS_SUPPORTED              0x100
#define CPUID_SSSE3_IS_SUPPORTED            0x200
#define CPUID_CNXT_ID_IS_SUPPORTED          0x400
#define CPUID_SDBG_IS_SUPPORTED             0x800
#define CPUID_FMA_IS_SUPPORTED              0x1000
#define CPUID_CMPXCHG16B_IS_SUPPORTED       0x2000
#define CPUID_XTPR_UPDATE_IS_SUPPORTED      0x4000
#define CPUID_PCDM_IS_IS_SUPPORTED          0x8000
// Bit 16 reserved.
#define CPUID_PCID_IS_SUPPORTED             0x20000
#define CPUID_DCA_IS_SUPPORTED              0x40000
#define CPUID_SSE_4_1_IS_SUPPORTED          0x80000
#define CPUID_SSE_4_2_IS_SUPPORTED          0x100000
#define CPUID_X2APIC_IS_SUPPORTED           0x200000
#define CPUID_MOVBE_IS_SUPPORTED            0x400000
#define CPUID_POPCNT_IS_SUPPORTED           0x800000
#define CPUID_TSC_DEADLINE_IS_SUPPORTED     0x1000000
#define CPUID_AESNI_IS_SUPPORTED            0x2000000
#define CPUID_XSAVE_IS_SUPPORTED            0x4000000
#define CPUID_OSXSAVE_IS_SUPPORTED          0x8000000
#define CPUID_AVX_IS_SUPPORTED              0x10000000
#define CPUID_F16C_IS_SUPPORTED             0x20000000
#define CPUID_RDRAND_IS_SUPPORTED           0x40000000
// Bit 31 not used. Always returns 0. 0x80000000

// EDX
#define CPUID_FPU_IS_SUPPORTED     0x1
#define CPUID_VME_IS_SUPPORTED     0x2
#define CPUID_DE_IS_SUPPORTED      0x4
#define CPUID_PSE_IS_SUPPORTED     0x8
#define CPUID_TSC_IS_SUPPORTED     0x10
#define CPUID_MSR_IS_SUPPORTED     0x20
#define CPUID_PAE_IS_SUPPORTED     0x40
#define CPUID_MCE_IS_SUPPORTED     0x80
#define CPUID_CX8_IS_SUPPORTED     0x100
#define CPUID_APIC_IS_SUPPORTED    0x200
// Bit 10 reserved.
#define CPUID_SEP_IS_SUPPORTED     0x800
#define CPUID_MTRR_IS_SUPPORTED    0x1000
#define CPUID_PGE_IS_SUPPORTED     0x2000
#define CPUID_MCA_IS_SUPPORTED     0x4000
#define CPUID_CMOV_IS_SUPPORTED    0x8000
#define CPUID_PAT_IS_SUPPORTED     0x10000
#define CPUID_PSE_36_IS_SUPPORTED  0x20000
#define CPUID_PSN_IS_SUPPORTED     0x40000
#define CPUID_CLFSH_IS_SUPPORTED   0x80000
// Bit 20 reserved.
#define CPUID_DS_IS_SUPPORTED      0x200000
#define CPUID_ACPI_IS_SUPPORTED    0x400000
#define CPUID_MMX_IS_SUPPORTED     0x800000
#define CPUID_FXSR_IS_SUPPORTED    0x1000000
#define CPUID_SSE_IS_SUPPORTED     0x2000000
#define CPUID_SSE2_IS_SUPPORTED    0x4000000
#define CPUID_SS_IS_SUPPORTED      0x8000000
#define CPUID_HTT_IS_SUPPORTED     0x10000000
#define CPUID_TM_IS_SUPPORTED      0x20000000
// Bit 30 reserved.
#define CPUID_PBE_IS_SUPPORTED     0x80000000

// EAX = 02H
// TODO

// EAX = 03H
// NOTE: Processor serial number (PSN) is not supported in the Pentium 4 processor or later.
//       On all models, use the PSN flag (returned using CPUID) to check for PSN support before
//       accessing the feature.

// EAX
// Reserved.

// EBX
// Reserved.

// ECX
#define CPUID_BITS_00_TO_31_OF_PSN 0xffffffff // Bits 00-31 of 96 bit processor serial number.

// EDX
#define CPUID_BITS_32_TO_63_OF_PSN 0xffffffff // Bits 32-63 of 96 bit processor serial number.

// EAX = 04H

// EAX
#define CPUID_MORE_CACHES_AVAILABLE 0x1
#define CPUID_CACHE_DATA            0x2
#define CPUID_INSTRUCTION_CACHE     0x4
#define CPUID_UNIFIED_CACHE         0x8
// Bits 4-31 reserved.
// TODO: The documentation states that bits 4-31 are reserved but then continues with bits 5-31.
//       I'm not sure what to make of that.

// EBX
// TODO

// ECX
// TODO

// EDX
// TODO

// EAX = 05H
// TODO

// EAX = 06H
// TODO

// EAX = 07H

// EAX
#define CPUID_MAX_INPUT_VALUE_FOR_SUPPORTED_7_LEAF_SUB_LEAVES 0xffffffff

// EBX
#define CPUID_FSGSBASE_IS_SUPPORTED                 0x1
#define CPUID_IA32_TSC_ADJUST_MSR_IS_SUPPORTED      0x2
#define CPUID_SGX_IS_SUPPORTED                      0x4
#define CPUID_BMI1_IS_SUPPORTED                     0x8
#define CPUID_HLE_IS_SUPPORTED                      0x10
#define CPUID_AVX2_IS_SUPPORTED                     0x20
#define CPUID_FDP_EXCPTN_ONLY                       0x40
#define CPUID_SMEP_IS_SUPPORTED                     0x80
#define CPUID_BMI2_IS_SUPPORTED                     0x100
#define CPUID_ENHANCED_REP_MOVSB_STOSB_IS_SUPPORTED 0x200
#define CPUID_INVPCID_IS_SUPPORTED                  0x400
#define CPUID_RTM_IS_SUPPORTED                      0x800
#define CPUID_RDT_M_IS_SUPPORTED                    0x1000
#define CPUID_FPU_CS_AND_FPU_DS_IS_DEPRECATED       0x2000
#define CPUID_MPX_IS_SUPPORTED                      0x4000
#define CPUID_RDT_A_IS_SUPPORTED                    0x8000
// Bit 17 reserved.
#define CPUID_RDSEED_IS_SUPPORTED                   0x20000
#define CPUID_ADX_IS_SUPPORTED                      0x40000
#define CPUID_SMAP_IS_SUPPORTED                     0x80000
// Bits 21-22 reserved.
#define CPUID_CLFLUSHOPT_IS_SUPPORTED               0x400000
#define CPUID_CLWB_IS_SUPPORTED                     0x800000
#define CPUID_INTEL_PROCESSOR_TRACE_IS_SUPPORTED    0x1000000
// Bits 26-28 reserved.
#define CPUID_SHA_IS_SUPPORTED                      0x10000000
// Bits 30-31 reserved.

// ECX
#define CPUID_PREFETCHWT1_IS_SUPPORTED 0x1
// Bit 1 reserved.
#define CPUID_UMIP_IS_SUPPORTED        0x4
#define CPUID_PKU_IS_SUPPORTED         0x8
#define CPUID_OSPKE_IS_SUPPORTED       0x10
// Bits 5-16 reserved.
#define CPUID_VALUE_OF_MAWAU           0x1e0000
#define CPUID_RDPID_IS_SUPPORTED       0x200000
// Bits 23-29 reserved.
#define CPUID_SGX_LC_IS_SUPPORTED      0x20000000
// Bit 31 reserved.

// EDX
// Reserved.

// EAX = 09H
// TODO

// EAX = 0AH
// TODO

// EAX = 0BH
// TODO

// EAX = 0DH, ECX = 0
// TODO

// EAX = 0DH, ECX = 1
// TODO

// EAX = 0DH, ECX = n, n > 1
// TODO

// EAX = 0FH, ECX = 0
// TODO

// EAX = 0FH, ECX = 1
// TODO

// EAX = 10H, ECX = 0
// TODO

// EAX = 10H, ECX = ResID = 1
// TODO

// EAX = 10H, ECX = ResID = 2
// TODO

// EAX = 12H, ECX = 0
// TODO

// EAX = 12H, ECX = 1
// TODO

// EAX = 12H, ECX = 2 or higher
// TODO

// EAX = 14H, ECX = 0
// TODO

// EAX = 14H, ECX = 1
// TODO

// EAX = 15H
// TODO

// EAX = 16H
// TODO

// EAX = 17H, ECX = 0
// TODO

// EAX = 17H, ECX = 1..3
// TODO

// EAX = 17H, ECX > MaxSOCID_Index
// TODO

// EAX = 80000000H
// TODO

// EAX = 80000001H
// TODO

// EAX = 80000002H
// TODO

// EAX = 80000003H
// TODO

// EAX = 80000004H
// TODO

// EAX = 80000005H
// TODO

// EAX = 80000006H
// TODO

// EAX = 80000007H
// TODO

// EAX = 80000008H
// TODO

#endif // !CPUID_H

#ifdef CPUID_IMPLEMENTATION

#ifdef _MSC_VER
#include <intrin.h>

void cpuid_get_cpuid(int eax, int *cpuinfo)
{
    __cpuid(cpuinfo, eax);
}
#elif defined(__GNUC__)
#include <cpuid.h>

void cpuid_get_cpuid(int eax, int *cpuinfo)
{
    __cpuid(eax, cpuinfo[0], cpuinfo[1], cpuinfo[2], cpuinfo[3]);
}
#else
#error "The compiler you're using is currently not supported (only MSVC, GCC and Clang are.)"
#endif

// EAX = 0H

// EBX + ECX + EDX
// Returns a twelve + null byte = 13 byte long string. Possible values:
// - "GenuineIntel" (Intel)
// - "AuthenticAMD" (AMD)
// - "CyrixInstead" (Cyrix)
// - "CentaurHauls" (VIA)
// - "GenuineTMx86" (Transmeta)
// - "SiS SiS SiS " (SIS)
// - "UMC UMC UMC " (UMC)
void cpuid_get_cpu_vendor_string(int *cpuinfo, char *str)
{
    str[0]  = cpuinfo[1]       & 0xff;
    str[1]  = cpuinfo[1] >> 8  & 0xff;
    str[2]  = cpuinfo[1] >> 16 & 0xff;
    str[3]  = cpuinfo[1] >> 24 & 0xff;

    str[4]  = cpuinfo[3]       & 0xff;
    str[5]  = cpuinfo[3] >> 8  & 0xff;
    str[6]  = cpuinfo[3] >> 16 & 0xff;
    str[7]  = cpuinfo[3] >> 24 & 0xff;

    str[8]  = cpuinfo[2]       & 0xff;
    str[9]  = cpuinfo[2] >> 8  & 0xff;
    str[10] = cpuinfo[2] >> 16 & 0xff;
    str[11] = cpuinfo[2] >> 24 & 0xff;

    str[12] = 0; // This is debatable.
}

#endif // !CPUID_IMPLEMENTATION
