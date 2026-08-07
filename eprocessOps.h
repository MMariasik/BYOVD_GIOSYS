#pragma once

#include <Windows.h>

// pretty funny I spent over an hour to fill these declarations to find out that  PsLookupProcessByProcessId() is part of WDK, so i cant use it inside normal program

//0x1c8 bytes (sizeof)

//0x18 bytes (sizeof)
struct _DISPATCHER_HEADER
{
    union
    {
        volatile LONG Lock;                                                 //0x0
        LONG LockNV;                                                        //0x0
        struct
        {
            UCHAR Type;                                                     //0x0
            UCHAR Signalling;                                               //0x1
            UCHAR Size;                                                     //0x2
            UCHAR Reserved1;                                                //0x3
        };
        struct
        {
            UCHAR TimerType;                                                //0x0
            union
            {
                UCHAR TimerControlFlags;                                    //0x1
                struct
                {
                    UCHAR Absolute : 1;                                       //0x1
                    UCHAR Wake : 1;                                           //0x1
                    UCHAR EncodedTolerableDelay : 6;                          //0x1
                };
            };
            UCHAR Hand;                                                     //0x2
            union
            {
                UCHAR TimerMiscFlags;                                       //0x3
                struct
                {
                    UCHAR Index : 6;                                          //0x3
                    UCHAR Inserted : 1;                                       //0x3
                    volatile UCHAR Expired : 1;                               //0x3
                };
            };
        };
        struct
        {
            UCHAR Timer2Type;                                               //0x0
            union
            {
                UCHAR Timer2Flags;                                          //0x1
                struct
                {
                    UCHAR Timer2Inserted : 1;                                 //0x1
                    UCHAR Timer2Expiring : 1;                                 //0x1
                    UCHAR Timer2CancelPending : 1;                            //0x1
                    UCHAR Timer2SetPending : 1;                               //0x1
                    UCHAR Timer2Running : 1;                                  //0x1
                    UCHAR Timer2Disabled : 1;                                 //0x1
                    UCHAR Timer2ReservedFlags : 2;                            //0x1
                };
            };
            UCHAR Timer2ComponentId;                                        //0x2
            UCHAR Timer2RelativeId;                                         //0x3
        };
        struct
        {
            UCHAR QueueType;                                                //0x0
            union
            {
                UCHAR QueueControlFlags;                                    //0x1
                struct
                {
                    UCHAR Abandoned : 1;                                      //0x1
                    UCHAR DisableIncrement : 1;                               //0x1
                    UCHAR QueueReservedControlFlags : 6;                      //0x1
                };
            };
            UCHAR QueueSize;                                                //0x2
            UCHAR QueueReserved;                                            //0x3
        };
        struct
        {
            UCHAR ThreadType;                                               //0x0
            UCHAR ThreadReserved;                                           //0x1
            union
            {
                UCHAR ThreadControlFlags;                                   //0x2
                struct
                {
                    UCHAR CycleProfiling : 1;                                 //0x2
                    UCHAR CounterProfiling : 1;                               //0x2
                    UCHAR GroupScheduling : 1;                                //0x2
                    UCHAR AffinitySet : 1;                                    //0x2
                    UCHAR Tagged : 1;                                         //0x2
                    UCHAR EnergyProfiling : 1;                                //0x2
                    UCHAR SchedulerAssist : 1;                                //0x2
                    UCHAR ThreadReservedControlFlags : 1;                     //0x2
                };
            };
            union
            {
                UCHAR DebugActive;                                          //0x3
                struct
                {
                    UCHAR ActiveDR7 : 1;                                      //0x3
                    UCHAR Instrumented : 1;                                   //0x3
                    UCHAR Minimal : 1;                                        //0x3
                    UCHAR Reserved4 : 2;                                      //0x3
                    UCHAR AltSyscall : 1;                                     //0x3
                    UCHAR Emulation : 1;                                      //0x3
                    UCHAR Reserved5 : 1;                                      //0x3
                };
            };
        };
        struct
        {
            UCHAR MutantType;                                               //0x0
            UCHAR MutantSize;                                               //0x1
            UCHAR DpcActive;                                                //0x2
            UCHAR MutantReserved;                                           //0x3
        };
    };
    LONG SignalState;                                                       //0x4
    struct _LIST_ENTRY WaitListHead;                                        //0x8
};

//0x10 bytes (sizeof)
struct _KAB_UM_PROCESS_CONTEXT
{
    struct _KAB_UM_PROCESS_TREE* Trees;                                     //0x0
    ULONG TreeCount;                                                        //0x8
};

//0x1 bytes (sizeof)
union _KEXECUTE_OPTIONS
{
    UCHAR ExecuteDisable : 1;                                                 //0x0
    UCHAR ExecuteEnable : 1;                                                  //0x0
    UCHAR DisableThunkEmulation : 1;                                          //0x0
    UCHAR Permanent : 1;                                                      //0x0
    UCHAR ExecuteDispatchEnable : 1;                                          //0x0
    UCHAR ImageDispatchEnable : 1;                                            //0x0
    UCHAR DisableExceptionChainValidation : 1;                                //0x0
    UCHAR Spare : 1;                                                          //0x0
    volatile UCHAR ExecuteOptions;                                          //0x0
    UCHAR ExecuteOptionsNV;                                                 //0x0
};

//0x10 bytes (sizeof)
struct _KGROUP_MASK
{
    ULONGLONG Masks[2];                                                     //0x0
};

//0x4 bytes (sizeof)
union _KSTACK_COUNT
{
    LONG Value;                                                             //0x0
    ULONG State : 3;                                                          //0x0
    ULONG StackCount : 29;                                                    //0x0
};

struct _KPROCESS
{
    struct _DISPATCHER_HEADER Header;                                       //0x0
    struct _LIST_ENTRY ProfileListHead;                                     //0x18
    ULONGLONG DirectoryTableBase;                                           //0x28
    struct _LIST_ENTRY ThreadListHead;                                      //0x30
    ULONG ProcessLock;                                                      //0x40
    ULONG ProcessTimerDelay;                                                //0x44
    ULONGLONG DeepFreezeStartTime;                                          //0x48
    struct _KAFFINITY_EX* Affinity;                                         //0x50
    struct _KAB_UM_PROCESS_CONTEXT AutoBoostState;                          //0x58
    struct _LIST_ENTRY ReadyListHead;                                       //0x68
    struct _SINGLE_LIST_ENTRY SwapListEntry;                                //0x78
    struct _KAFFINITY_EX* ActiveProcessors;                                 //0x80
    union
    {
        struct
        {
            ULONG AutoAlignment : 1;                                          //0x88
            ULONG DisableBoost : 1;                                           //0x88
            ULONG DisableQuantum : 1;                                         //0x88
            ULONG DeepFreeze : 1;                                             //0x88
            ULONG TimerVirtualization : 1;                                    //0x88
            ULONG CheckStackExtents : 1;                                      //0x88
            ULONG CacheIsolationEnabled : 1;                                  //0x88
            ULONG PpmPolicy : 4;                                              //0x88
            ULONG VaSpaceDeleted : 1;                                         //0x88
            ULONG MultiGroup : 1;                                             //0x88
            ULONG ForegroundProcess : 1;                                      //0x88
            ULONG ReservedFlags : 18;                                         //0x88
        };
        volatile LONG ProcessFlags;                                         //0x88
    };
    ULONG Spare0c;                                                          //0x8c
    CHAR BasePriority;                                                      //0x90
    CHAR QuantumReset;                                                      //0x91
    CHAR Visited;                                                           //0x92
    union _KEXECUTE_OPTIONS Flags;                                          //0x93
    struct _KGROUP_MASK ActiveGroupsMask;                                   //0x98
    ULONGLONG ActiveGroupPadding[2];                                        //0xa8
    struct _KI_IDEAL_PROCESSOR_ASSIGNMENT_BLOCK* IdealProcessorAssignmentBlock; //0xb8
    ULONGLONG Padding[6];                                                   //0xc0
    ULONG Padding2;                                                         //0xf0
    ULONG SchedulerAssistYieldBoostCount;                                   //0xf4
    LONGLONG SchedulerAssistYieldBoostAllowedTime;                          //0xf8
    ULONG Spare0d;                                                          //0x100
    USHORT IdealGlobalNode;                                                 //0x104
    USHORT Spare1;                                                          //0x106
    union _KSTACK_COUNT StackCount;                                 //0x108
    struct _LIST_ENTRY ProcessListEntry;                                    //0x110
    ULONGLONG CycleTime;                                                    //0x120
    ULONGLONG ContextSwitches;                                              //0x128
    struct _KSCHEDULING_GROUP* SchedulingGroup;                             //0x130
    ULONGLONG KernelTime;                                                   //0x138
    ULONGLONG UserTime;                                                     //0x140
    ULONGLONG ReadyTime;                                                    //0x148
    ULONG FreezeCount;                                                      //0x150
    ULONG Spare4;                                                           //0x154
    ULONGLONG UserDirectoryTableBase;                                       //0x158
    UCHAR AddressPolicy;                                                    //0x160
    UCHAR Spare2[7];                                                        //0x161
    VOID* InstrumentationCallback;                                          //0x168
    union
    {
        ULONGLONG SecureHandle;                                             //0x170
        struct
        {
            ULONGLONG SecureProcess : 1;                                      //0x170
            ULONGLONG TrustedApp : 1;                                         //0x170
        } Flags;                                                            //0x170
    } SecureState;                                                          //0x170
    ULONGLONG KernelWaitTime;                                               //0x178
    ULONGLONG UserWaitTime;                                                 //0x180
    ULONGLONG LastRebalanceQpc;                                             //0x188
    VOID* PerProcessorCycleTimes;                                           //0x190
    ULONGLONG ExtendedFeatureDisableMask;                                   //0x198
    USHORT PrimaryGroup;                                                    //0x1a0
    USHORT Spare3[3];                                                       //0x1a2
    VOID* UserCetLogging;                                                   //0x1a8
    struct _LIST_ENTRY CpuPartitionList;                                    //0x1b0
    struct _KPROCESS_AVAILABLE_CPU_STATE* AvailableCpuState;                //0x1c0
};

//0x8 bytes (sizeof)
struct _EX_PUSH_LOCK
{
    union
    {
        struct
        {
            ULONGLONG Locked : 1;                                             //0x0
            ULONGLONG Waiting : 1;                                            //0x0
            ULONGLONG Waking : 1;                                             //0x0
            ULONGLONG MultipleShared : 1;                                     //0x0
            ULONGLONG Shared : 60;                                            //0x0
        };
        ULONGLONG Value;                                                    //0x0
        VOID* Ptr;                                                          //0x0
    };
};

//0x8 bytes (sizeof)
struct _EX_RUNDOWN_REF
{
    union
    {
        ULONGLONG Count;                                                    //0x0
        VOID* Ptr;                                                          //0x0
    };
};

//0x8 bytes (sizeof)
struct _EX_FAST_REF
{
    union
    {
        VOID* Object;                                                       //0x0
        ULONGLONG RefCnt : 4;                                                 //0x0
        ULONGLONG Value;                                                    //0x0
    };
};

//0x8 bytes (sizeof)
struct _RTL_AVL_TREE
{
    struct _RTL_BALANCED_NODE* Root;                                        //0x0
};

//0x8 bytes (sizeof)
struct _SE_AUDIT_PROCESS_CREATION_INFO
{
    struct _OBJECT_NAME_INFORMATION* ImageFileName;                         //0x0
};

//0x8 bytes (sizeof)
struct _JOBOBJECT_WAKE_FILTER
{
    ULONG HighEdgeFilter;                                                   //0x0
    ULONG LowEdgeFilter;                                                    //0x4
};

//0x30 bytes (sizeof)
struct _PS_PROCESS_WAKE_INFORMATION
{
    ULONGLONG NotificationChannel;                                          //0x0
    ULONG WakeCounters[7];                                                  //0x8
    struct _JOBOBJECT_WAKE_FILTER WakeFilter;                               //0x24
    ULONG NoWakeCounter;                                                    //0x2c
};

//0x8 bytes (sizeof)
struct _WNF_STATE_NAME
{
    ULONG Data[2];                                                          //0x0
};

//0x8 bytes (sizeof)
union _PS_INTERLOCKED_TIMER_DELAY_VALUES
{
    ULONGLONG DelayMs : 30;                                                   //0x0
    ULONGLONG CoalescingWindowMs : 30;                                        //0x0
    ULONGLONG Reserved : 1;                                                   //0x0
    ULONGLONG NewTimerWheel : 1;                                              //0x0
    ULONGLONG Retry : 1;                                                      //0x0
    ULONGLONG Locked : 1;                                                     //0x0
    ULONGLONG All;                                                          //0x0
};

//0x1 bytes (sizeof)
struct _PS_PROTECTION
{
    union
    {
        UCHAR Level;                                                        //0x0
        struct
        {
            UCHAR Type : 3;                                                   //0x0
            UCHAR Audit : 1;                                                  //0x0
            UCHAR Signer : 4;                                                 //0x0
        };
    };
};

//0x20 bytes (sizeof)
struct _ALPC_PROCESS_CONTEXT
{
    struct _EX_PUSH_LOCK Lock;                                              //0x0
    struct _LIST_ENTRY ViewListHead;                                        //0x8
    volatile ULONGLONG PagedPoolQuotaCache;                                 //0x18
};

//0x4 bytes (sizeof)
struct _MMSUPPORT_FLAGS
{
    union
    {
        struct
        {
            UCHAR WorkingSetType : 4;                                         //0x0
            UCHAR Reserved0 : 2;                                              //0x0
            UCHAR MaximumWorkingSetHard : 1;                                  //0x0
            UCHAR MinimumWorkingSetHard : 1;                                  //0x0
            UCHAR Reserved1 : 1;                                              //0x1
            UCHAR TrimmerState : 2;                                           //0x1
            UCHAR LinearAddressProtected : 1;                                 //0x1
            UCHAR PageStealers : 4;                                           //0x1
        };
        struct
        {
            USHORT u1;                                                      //0x0
            UCHAR MemoryPriority;                                           //0x2
            union
            {
                struct
                {
                    UCHAR WsleDeleted : 1;                                    //0x3
                    UCHAR SvmEnabled : 1;                                     //0x3
                    UCHAR ForceAge : 1;                                       //0x3
                    UCHAR ForceTrim : 1;                                      //0x3
                    UCHAR CommitReleaseState : 2;                             //0x3
                    UCHAR Reserved2 : 2;                                      //0x3
                };
                UCHAR u2;                                                   //0x3
            };
        };
        ULONG EntireFlags;                                                  //0x0
    };
};

//0xc0 bytes (sizeof)
struct _MMSUPPORT_INSTANCE
{
    ULONG NextPageColor;                                                    //0x0
    volatile ULONG PageFaultCount;                                          //0x4
    ULONGLONG TrimmedPageCount;                                             //0x8
    struct _MMWSL_INSTANCE* VmWorkingSetList;                               //0x10
    struct _LIST_ENTRY WorkingSetExpansionLinks;                            //0x18
    volatile ULONGLONG AgeDistribution[8];                                  //0x28
    struct _KGATE* ExitOutswapGate;                                         //0x68
    ULONGLONG MinimumWorkingSetSize;                                        //0x70
    ULONGLONG MaximumWorkingSetSize;                                        //0x78
    volatile ULONGLONG WorkingSetLeafSize;                                  //0x80
    volatile ULONGLONG WorkingSetLeafPrivateSize;                           //0x88
    volatile ULONGLONG WorkingSetSize;                                      //0x90
    volatile ULONGLONG WorkingSetPrivateSize;                               //0x98
    volatile ULONGLONG PeakWorkingSetSize;                                  //0xa0
    ULONG HardFaultCount;                                                   //0xa8
    USHORT LastTrimStamp;                                                   //0xac
    USHORT PartitionId;                                                     //0xae
    ULONGLONG SelfmapLock;                                                  //0xb0
    volatile struct _MMSUPPORT_FLAGS Flags;                                 //0xb8
    volatile ULONG InterlockedFlags;                                        //0xbc
};

//0x80 bytes (sizeof)
struct _MMSUPPORT_SHARED
{
    VOID* WorkingSetLockArray;                                              //0x0
    ULONGLONG ReleasedCommitDebt;                                           //0x8
    ULONGLONG ResetPagesRepurposedCount;                                    //0x10
    VOID* WsSwapSupport;                                                    //0x18
    VOID* CommitReleaseContext;                                             //0x20
    VOID* AccessLog;                                                        //0x28
    volatile ULONGLONG ChargedWslePages;                                    //0x30
    volatile ULONGLONG ActualWslePages;                                     //0x38
    volatile LONG WorkingSetCoreLock;                                       //0x40
    VOID* ShadowMapping;                                                    //0x48
};

//0x140 bytes (sizeof)
struct _MMSUPPORT_FULL
{
    struct _MMSUPPORT_INSTANCE Instance;                                    //0x0
    struct _MMSUPPORT_SHARED Shared;                                        //0xc0
};

//0x2 bytes (sizeof)
union _PROCESS_EXECUTION_TRANSITION
{
    volatile SHORT TransitionState;                                         //0x0
    USHORT InProgress : 1;                                                    //0x0
    USHORT Reserved : 7;                                                      //0x0
};

//0x1 bytes (sizeof)
union _PROCESS_EXECUTION_STATE
{
    CHAR State;                                                             //0x0
    UCHAR ProcessFrozen : 1;                                                  //0x0
    UCHAR ProcessSwapped : 1;                                                 //0x0
    UCHAR ProcessGraphicsFreezeOptimized : 1;                                 //0x0
    UCHAR Reserved : 5;                                                       //0x0
};

//0x4 bytes (sizeof)
union _PROCESS_EXECUTION
{
    volatile LONG State;                                                    //0x0
    struct
    {
        union _PROCESS_EXECUTION_TRANSITION Transition;             //0x0
        union _PROCESS_EXECUTION_STATE Current;                             //0x2
    };
    union _PROCESS_EXECUTION_STATE Requested;                               //0x3
};

//0x8 bytes (sizeof)
struct _PSP_SYSCALL_PROVIDER_DISPATCH_CONTEXT
{
    ULONG Level;                                                            //0x0
    ULONG Slot;                                                             //0x4
};

//0x10 bytes (sizeof)
struct _PS_DYNAMIC_ENFORCED_ADDRESS_RANGES
{
    struct _RTL_AVL_TREE Tree;                                              //0x0
    struct _EX_PUSH_LOCK Lock;                                              //0x8
};


//0x840 bytes (sizeof)
typedef struct _EPROCESS
{
    struct _KPROCESS Pcb;                                                   //0x0
    struct _EX_PUSH_LOCK ProcessLock;                                       //0x1c8
    VOID* UniqueProcessId;                                                  //0x1d0
    struct _LIST_ENTRY ActiveProcessLinks;                                  //0x1d8
    struct _EX_RUNDOWN_REF RundownProtect;                                  //0x1e8
    union
    {
        ULONG Flags2;                                                       //0x1f0
        struct
        {
            ULONG JobNotReallyActive : 1;                                     //0x1f0
            ULONG AccountingFolded : 1;                                       //0x1f0
            ULONG NewProcessReported : 1;                                     //0x1f0
            ULONG ExitProcessReported : 1;                                    //0x1f0
            ULONG ReportCommitChanges : 1;                                    //0x1f0
            ULONG LastReportMemory : 1;                                       //0x1f0
            ULONG ForceWakeCharge : 1;                                        //0x1f0
            ULONG CrossSessionCreate : 1;                                     //0x1f0
            ULONG NeedsHandleRundown : 1;                                     //0x1f0
            ULONG RefTraceEnabled : 1;                                        //0x1f0
            ULONG PicoCreated : 1;                                            //0x1f0
            ULONG EmptyJobEvaluated : 1;                                      //0x1f0
            ULONG DefaultPagePriority : 3;                                    //0x1f0
            ULONG PrimaryTokenFrozen : 1;                                     //0x1f0
            ULONG ProcessVerifierTarget : 1;                                  //0x1f0
            ULONG RestrictSetThreadContext : 1;                               //0x1f0
            ULONG AffinityPermanent : 1;                                      //0x1f0
            ULONG AffinityUpdateEnable : 1;                                   //0x1f0
            ULONG PropagateNode : 1;                                          //0x1f0
            ULONG ExplicitAffinity : 1;                                       //0x1f0
            ULONG Flags2Available1 : 2;                                       //0x1f0
            ULONG EnableReadVmLogging : 1;                                    //0x1f0
            ULONG EnableWriteVmLogging : 1;                                   //0x1f0
            ULONG FatalAccessTerminationRequested : 1;                        //0x1f0
            ULONG DisableSystemAllowedCpuSet : 1;                             //0x1f0
            ULONG Flags2Available2 : 3;                                       //0x1f0
            ULONG InPrivate : 1;                                              //0x1f0
        };
    };
    union
    {
        ULONG Flags;                                                        //0x1f4
        struct
        {
            ULONG CreateReported : 1;                                         //0x1f4
            ULONG NoDebugInherit : 1;                                         //0x1f4
            ULONG ProcessExiting : 1;                                         //0x1f4
            ULONG ProcessDelete : 1;                                          //0x1f4
            ULONG ManageExecutableMemoryWrites : 1;                           //0x1f4
            ULONG VmDeleted : 1;                                              //0x1f4
            ULONG OutswapEnabled : 1;                                         //0x1f4
            ULONG Outswapped : 1;                                             //0x1f4
            ULONG FailFastOnCommitFail : 1;                                   //0x1f4
            ULONG Wow64VaSpace4Gb : 1;                                        //0x1f4
            ULONG AddressSpaceInitialized : 2;                                //0x1f4
            ULONG SetTimerResolution : 1;                                     //0x1f4
            ULONG BreakOnTermination : 1;                                     //0x1f4
            ULONG DeprioritizeViews : 1;                                      //0x1f4
            ULONG WriteWatch : 1;                                             //0x1f4
            ULONG ProcessInSession : 1;                                       //0x1f4
            ULONG OverrideAddressSpace : 1;                                   //0x1f4
            ULONG HasAddressSpace : 1;                                        //0x1f4
            ULONG LaunchPrefetched : 1;                                       //0x1f4
            ULONG Reserved : 1;                                               //0x1f4
            ULONG VmTopDown : 1;                                              //0x1f4
            ULONG ImageNotifyDone : 1;                                        //0x1f4
            ULONG PdeUpdateNeeded : 1;                                        //0x1f4
            ULONG VdmAllowed : 1;                                             //0x1f4
            ULONG ProcessRundown : 1;                                         //0x1f4
            ULONG ProcessInserted : 1;                                        //0x1f4
            ULONG DefaultIoPriority : 3;                                      //0x1f4
            ULONG ProcessSelfDelete : 1;                                      //0x1f4
            ULONG SetTimerResolutionLink : 1;                                 //0x1f4
        };
    };
    union _LARGE_INTEGER CreateTime;                                        //0x1f8
    ULONGLONG ProcessQuotaUsage[2];                                         //0x200
    ULONGLONG ProcessQuotaPeak[2];                                          //0x210
    ULONGLONG PeakVirtualSize;                                              //0x220
    ULONGLONG VirtualSize;                                                  //0x228
    struct _LIST_ENTRY SessionProcessLinks;                                 //0x230
    union
    {
        VOID* ExceptionPortData;                                            //0x240
        ULONGLONG ExceptionPortValue;                                       //0x240
        ULONGLONG ExceptionPortState : 3;                                     //0x240
    };
    struct _EX_FAST_REF Token;                                              //0x248
    ULONGLONG MmReserved;                                                   //0x250
    struct _EX_PUSH_LOCK AddressCreationLock;                               //0x258
    struct _EX_PUSH_LOCK PageTableCommitmentLock;                           //0x260
    struct _ETHREAD* RotateInProgress;                                      //0x268
    struct _ETHREAD* ForkInProgress;                                        //0x270
    struct _EJOB* volatile CommitChargeJob;                                 //0x278
    struct _RTL_AVL_TREE CloneRoot;                                         //0x280
    volatile ULONGLONG NumberOfPrivatePages;                                //0x288
    volatile ULONGLONG NumberOfLockedPages;                                 //0x290
    VOID* Win32Process;                                                     //0x298
    struct _EJOB* volatile Job;                                             //0x2a0
    VOID* SectionObject;                                                    //0x2a8
    VOID* SectionBaseAddress;                                               //0x2b0
    ULONG Cookie;                                                           //0x2b8
    struct _PAGEFAULT_HISTORY* WorkingSetWatch;                             //0x2c0
    VOID* Win32WindowStation;                                               //0x2c8
    VOID* InheritedFromUniqueProcessId;                                     //0x2d0
    volatile ULONGLONG OwnerProcessId;                                      //0x2d8
    struct _PEB* Peb;                                                       //0x2e0
    struct _PSP_SESSION_SPACE* Session;                                     //0x2e8
    VOID* Spare1;                                                           //0x2f0
    struct _EPROCESS_QUOTA_BLOCK* QuotaBlock;                               //0x2f8
    struct _HANDLE_TABLE* ObjectTable;                                      //0x300
    VOID* DebugPort;                                                        //0x308
    struct _EWOW64PROCESS* WoW64Process;                                    //0x310
    struct _EX_FAST_REF DeviceMap;                                          //0x318
    VOID* EtwDataSource;                                                    //0x320
    ULONGLONG PageDirectoryPte;                                             //0x328
    struct _FILE_OBJECT* ImageFilePointer;                                  //0x330
    UCHAR ImageFileName[15];                                                //0x338
    UCHAR PriorityClass;                                                    //0x347
    VOID* SecurityPort;                                                     //0x348
    struct _SE_AUDIT_PROCESS_CREATION_INFO SeAuditProcessCreationInfo;      //0x350
    struct _LIST_ENTRY JobLinks;                                            //0x358
    VOID* HighestUserAddress;                                               //0x368
    struct _LIST_ENTRY ThreadListHead;                                      //0x370
    volatile ULONG ActiveThreads;                                           //0x380
    ULONG ImagePathHash;                                                    //0x384
    ULONG DefaultHardErrorProcessing;                                       //0x388
    LONG LastThreadExitStatus;                                              //0x38c
    struct _EX_FAST_REF PrefetchTrace;                                      //0x390
    VOID* LockedPagesList;                                                  //0x398
    union _LARGE_INTEGER ReadOperationCount;                                //0x3a0
    union _LARGE_INTEGER WriteOperationCount;                               //0x3a8
    union _LARGE_INTEGER OtherOperationCount;                               //0x3b0
    union _LARGE_INTEGER ReadTransferCount;                                 //0x3b8
    union _LARGE_INTEGER WriteTransferCount;                                //0x3c0
    union _LARGE_INTEGER OtherTransferCount;                                //0x3c8
    ULONGLONG CommitChargeLimit;                                            //0x3d0
    volatile ULONGLONG CommitCharge;                                        //0x3d8
    volatile ULONGLONG CommitChargePeak;                                    //0x3e0
    struct _MMSUPPORT_FULL Vm;                                              //0x400
    struct _LIST_ENTRY MmProcessLinks;                                      //0x540
    volatile ULONG ModifiedPageCount;                                       //0x550
    LONG ExitStatus;                                                        //0x554
    struct _RTL_AVL_TREE VadRoot;                                           //0x558
    VOID* VadHint;                                                          //0x560
    ULONGLONG VadCount;                                                     //0x568
    volatile ULONGLONG VadPhysicalPages;                                    //0x570
    ULONGLONG VadPhysicalPagesLimit;                                        //0x578
    struct _ALPC_PROCESS_CONTEXT AlpcContext;                               //0x580
    struct _LIST_ENTRY TimerResolutionLink;                                 //0x5a0
    struct _PO_DIAG_STACK_RECORD* TimerResolutionStackRecord;               //0x5b0
    ULONG RequestedTimerResolution;                                         //0x5b8
    ULONG SmallestTimerResolution;                                          //0x5bc
    union _LARGE_INTEGER ExitTime;                                          //0x5c0
    struct _INVERTED_FUNCTION_TABLE_KERNEL_MODE* InvertedFunctionTable;     //0x5c8
    struct _EX_PUSH_LOCK InvertedFunctionTableLock;                         //0x5d0
    ULONG ActiveThreadsHighWatermark;                                       //0x5d8
    ULONG LargePrivateVadCount;                                             //0x5dc
    struct _EX_PUSH_LOCK ThreadListLock;                                    //0x5e0
    VOID* WnfContext;                                                       //0x5e8
    struct _EJOB* ServerSilo;                                               //0x5f0
    UCHAR SignatureLevel;                                                   //0x5f8
    UCHAR SectionSignatureLevel;                                            //0x5f9
    struct _PS_PROTECTION Protection;                                       //0x5fa
    UCHAR HangCount : 3;                                                      //0x5fb
    UCHAR GhostCount : 3;                                                     //0x5fb
    UCHAR PrefilterException : 1;                                             //0x5fb
    union
    {
        ULONG Flags3;                                                       //0x5fc
        struct
        {
            ULONG Minimal : 1;                                                //0x5fc
            ULONG ReplacingPageRoot : 1;                                      //0x5fc
            ULONG Crashed : 1;                                                //0x5fc
            ULONG JobVadsAreTracked : 1;                                      //0x5fc
            ULONG VadTrackingDisabled : 1;                                    //0x5fc
            ULONG AuxiliaryProcess : 1;                                       //0x5fc
            ULONG SubsystemProcess : 1;                                       //0x5fc
            ULONG IndirectCpuSets : 1;                                        //0x5fc
            ULONG RelinquishedCommit : 1;                                     //0x5fc
            ULONG HighGraphicsPriority : 1;                                   //0x5fc
            ULONG CommitFailLogged : 1;                                       //0x5fc
            ULONG ReserveFailLogged : 1;                                      //0x5fc
            ULONG SystemProcess : 1;                                          //0x5fc
            ULONG AllImagesAtBasePristineBase : 1;                            //0x5fc
            ULONG AddressPolicyFrozen : 1;                                    //0x5fc
            ULONG ProcessFirstResume : 1;                                     //0x5fc
            ULONG ForegroundExternal : 1;                                     //0x5fc
            ULONG ForegroundSystem : 1;                                       //0x5fc
            ULONG HighMemoryPriority : 1;                                     //0x5fc
            ULONG EnableProcessSuspendResumeLogging : 1;                      //0x5fc
            ULONG EnableThreadSuspendResumeLogging : 1;                       //0x5fc
            ULONG SecurityDomainChanged : 1;                                  //0x5fc
            ULONG SecurityFreezeComplete : 1;                                 //0x5fc
            ULONG VmProcessorHost : 1;                                        //0x5fc
            ULONG VmProcessorHostTransition : 1;                              //0x5fc
            ULONG AltSyscall : 1;                                             //0x5fc
            ULONG TimerResolutionIgnore : 1;                                  //0x5fc
            ULONG DisallowUserTerminate : 1;                                  //0x5fc
            ULONG EnableProcessRemoteExecProtectVmLogging : 1;                //0x5fc
            ULONG EnableProcessLocalExecProtectVmLogging : 1;                 //0x5fc
            ULONG MemoryCompressionProcess : 1;                               //0x5fc
            ULONG EnableProcessImpersonationLogging : 1;                      //0x5fc
        };
    };
    LONG DeviceAsid;                                                        //0x600
    VOID* SvmData;                                                          //0x608
    struct _EX_PUSH_LOCK SvmProcessLock;                                    //0x610
    ULONGLONG SvmLock;                                                      //0x618
    struct _LIST_ENTRY SvmProcessDeviceListHead;                            //0x620
    ULONGLONG LastFreezeInterruptTime;                                      //0x630
    struct _PROCESS_DISK_COUNTERS* DiskCounters;                            //0x638
    VOID* PicoContext;                                                      //0x640
    VOID* EnclaveTable;                                                     //0x648
    ULONGLONG EnclaveNumber;                                                //0x650
    struct _EX_PUSH_LOCK EnclaveLock;                                       //0x658
    ULONG HighPriorityFaultsAllowed;                                        //0x660
    struct _PO_PROCESS_ENERGY_CONTEXT* EnergyContext;                       //0x668
    VOID* VmContext;                                                        //0x670
    ULONGLONG SequenceNumber;                                               //0x678
    ULONGLONG CreateInterruptTime;                                          //0x680
    ULONGLONG CreateUnbiasedInterruptTime;                                  //0x688
    ULONGLONG TotalUnbiasedFrozenTime;                                      //0x690
    ULONGLONG LastAppStateUpdateTime;                                       //0x698
    ULONGLONG LastAppStateUptime : 61;                                        //0x6a0
    ULONGLONG LastAppState : 3;                                               //0x6a0
    volatile ULONGLONG SharedCommitCharge;                                  //0x6a8
    struct _EX_PUSH_LOCK SharedCommitLock;                                  //0x6b0
    struct _LIST_ENTRY SharedCommitLinks;                                   //0x6b8
    union
    {
        struct
        {
            ULONGLONG AllowedCpuSets;                                       //0x6c8
            ULONGLONG DefaultCpuSets;                                       //0x6d0
        };
        struct
        {
            ULONGLONG* AllowedCpuSetsIndirect;                              //0x6c8
            ULONGLONG* DefaultCpuSetsIndirect;                              //0x6d0
        };
    };
    VOID* DiskIoAttribution;                                                //0x6d8
    VOID* DxgProcess;                                                       //0x6e0
    ULONG Win32KFilterSet;                                                  //0x6e8
    USHORT Machine;                                                         //0x6ec
    UCHAR MmSlabIdentity;                                                   //0x6ee
    UCHAR Spare0;                                                           //0x6ef
    union _PS_INTERLOCKED_TIMER_DELAY_VALUES ProcessTimerDelay;     //0x6f0
    volatile ULONG KTimerSets;                                              //0x6f8
    volatile ULONG KTimer2Sets;                                             //0x6fc
    volatile ULONG ThreadTimerSets;                                         //0x700
    ULONGLONG VirtualTimerListLock;                                         //0x708
    struct _LIST_ENTRY VirtualTimerListHead;                                //0x710
    union
    {
        struct _WNF_STATE_NAME WakeChannel;                                 //0x720
        struct _PS_PROCESS_WAKE_INFORMATION WakeInfo;                       //0x720
    };
    union
    {
        ULONG MitigationFlags;                                              //0x750
        struct
        {
            ULONG ControlFlowGuardEnabled : 1;                                //0x750
            ULONG ControlFlowGuardExportSuppressionEnabled : 1;               //0x750
            ULONG ControlFlowGuardStrict : 1;                                 //0x750
            ULONG DisallowStrippedImages : 1;                                 //0x750
            ULONG ForceRelocateImages : 1;                                    //0x750
            ULONG HighEntropyASLREnabled : 1;                                 //0x750
            ULONG StackRandomizationDisabled : 1;                             //0x750
            ULONG ExtensionPointDisable : 1;                                  //0x750
            ULONG DisableDynamicCode : 1;                                     //0x750
            ULONG DisableDynamicCodeAllowOptOut : 1;                          //0x750
            ULONG DisableDynamicCodeAllowRemoteDowngrade : 1;                 //0x750
            ULONG AuditDisableDynamicCode : 1;                                //0x750
            ULONG DisallowWin32kSystemCalls : 1;                              //0x750
            ULONG AuditDisallowWin32kSystemCalls : 1;                         //0x750
            ULONG EnableFilteredWin32kAPIs : 1;                               //0x750
            ULONG AuditFilteredWin32kAPIs : 1;                                //0x750
            ULONG DisableNonSystemFonts : 1;                                  //0x750
            ULONG AuditNonSystemFontLoading : 1;                              //0x750
            ULONG PreferSystem32Images : 1;                                   //0x750
            ULONG ProhibitRemoteImageMap : 1;                                 //0x750
            ULONG AuditProhibitRemoteImageMap : 1;                            //0x750
            ULONG ProhibitLowILImageMap : 1;                                  //0x750
            ULONG AuditProhibitLowILImageMap : 1;                             //0x750
            ULONG SignatureMitigationOptIn : 1;                               //0x750
            ULONG AuditBlockNonMicrosoftBinaries : 1;                         //0x750
            ULONG AuditBlockNonMicrosoftBinariesAllowStore : 1;               //0x750
            ULONG LoaderIntegrityContinuityEnabled : 1;                       //0x750
            ULONG AuditLoaderIntegrityContinuity : 1;                         //0x750
            ULONG EnableModuleTamperingProtection : 1;                        //0x750
            ULONG EnableModuleTamperingProtectionNoInherit : 1;               //0x750
            ULONG RestrictIndirectBranchPrediction : 1;                       //0x750
            ULONG IsolateSecurityDomain : 1;                                  //0x750
        } MitigationFlagsValues;                                            //0x750
    };
    union
    {
        ULONG MitigationFlags2;                                             //0x754
        struct
        {
            ULONG EnableExportAddressFilter : 1;                              //0x754
            ULONG AuditExportAddressFilter : 1;                               //0x754
            ULONG EnableExportAddressFilterPlus : 1;                          //0x754
            ULONG AuditExportAddressFilterPlus : 1;                           //0x754
            ULONG EnableRopStackPivot : 1;                                    //0x754
            ULONG AuditRopStackPivot : 1;                                     //0x754
            ULONG EnableRopCallerCheck : 1;                                   //0x754
            ULONG AuditRopCallerCheck : 1;                                    //0x754
            ULONG EnableRopSimExec : 1;                                       //0x754
            ULONG AuditRopSimExec : 1;                                        //0x754
            ULONG EnableImportAddressFilter : 1;                              //0x754
            ULONG AuditImportAddressFilter : 1;                               //0x754
            ULONG DisablePageCombine : 1;                                     //0x754
            ULONG SpeculativeStoreBypassDisable : 1;                          //0x754
            ULONG CetUserShadowStacks : 1;                                    //0x754
            ULONG AuditCetUserShadowStacks : 1;                               //0x754
            ULONG AuditCetUserShadowStacksLogged : 1;                         //0x754
            ULONG UserCetSetContextIpValidation : 1;                          //0x754
            ULONG AuditUserCetSetContextIpValidation : 1;                     //0x754
            ULONG AuditUserCetSetContextIpValidationLogged : 1;               //0x754
            ULONG CetUserShadowStacksStrictMode : 1;                          //0x754
            ULONG BlockNonCetBinaries : 1;                                    //0x754
            ULONG BlockNonCetBinariesNonEhcont : 1;                           //0x754
            ULONG AuditBlockNonCetBinaries : 1;                               //0x754
            ULONG AuditBlockNonCetBinariesLogged : 1;                         //0x754
            ULONG XtendedControlFlowGuard_Deprecated : 1;                     //0x754
            ULONG AuditXtendedControlFlowGuard_Deprecated : 1;                //0x754
            ULONG PointerAuthUserIp : 1;                                      //0x754
            ULONG AuditPointerAuthUserIp : 1;                                 //0x754
            ULONG AuditPointerAuthUserIpLogged : 1;                           //0x754
            ULONG CetDynamicApisOutOfProcOnly : 1;                            //0x754
            ULONG UserCetSetContextIpValidationRelaxedMode : 1;               //0x754
        } MitigationFlags2Values;                                           //0x754
    };
    VOID* PartitionObject;                                                  //0x758
    ULONGLONG SecurityDomain;                                               //0x760
    ULONGLONG ParentSecurityDomain;                                         //0x768
    VOID* CoverageSamplerContext;                                           //0x770
    VOID* MmHotPatchContext;                                                //0x778
    struct _RTL_AVL_TREE DynamicEHContinuationTargetsTree;                  //0x780
    struct _EX_PUSH_LOCK DynamicEHContinuationTargetsLock;                  //0x788
    struct _PS_DYNAMIC_ENFORCED_ADDRESS_RANGES DynamicEnforcedCetCompatibleRanges; //0x790
    ULONG DisabledComponentFlags;                                           //0x7a0
    volatile LONG PageCombineSequence;                                      //0x7a4
    struct _EX_PUSH_LOCK EnableOptionalXStateFeaturesLock;                  //0x7a8
    ULONG* volatile PathRedirectionHashes;                                  //0x7b0
    struct _PS_SYSCALL_PROVIDER* SyscallProvider;                           //0x7b8
    struct _LIST_ENTRY SyscallProviderProcessLinks;                         //0x7c0
    struct _PSP_SYSCALL_PROVIDER_DISPATCH_CONTEXT SyscallProviderDispatchContext; //0x7d0
    union
    {
        ULONG MitigationFlags3;                                             //0x7d8
        struct
        {
            ULONG RestrictCoreSharing : 1;                                    //0x7d8
            ULONG DisallowFsctlSystemCalls : 1;                               //0x7d8
            ULONG AuditDisallowFsctlSystemCalls : 1;                          //0x7d8
            ULONG MitigationFlags3Spare : 29;                                 //0x7d8
        } MitigationFlags3Values;                                           //0x7d8
    };
    union
    {
        ULONG Flags4;                                                       //0x7dc
        struct
        {
            ULONG ThreadWasActive : 1;                                        //0x7dc
            ULONG MinimalTerminate : 1;                                       //0x7dc
            ULONG ImageExpansionDisable : 1;                                  //0x7dc
            ULONG SessionFirstProcess : 1;                                    //0x7dc
        };
    };
    union
    {
        ULONG SyscallUsage;                                                 //0x7e0
        struct
        {
            ULONG SystemModuleInformation : 1;                                //0x7e0
            ULONG SystemModuleInformationEx : 1;                              //0x7e0
            ULONG SystemLocksInformation : 1;                                 //0x7e0
            ULONG SystemStackTraceInformation : 1;                            //0x7e0
            ULONG SystemHandleInformation : 1;                                //0x7e0
            ULONG SystemExtendedHandleInformation : 1;                        //0x7e0
            ULONG SystemObjectInformation : 1;                                //0x7e0
            ULONG SystemBigPoolInformation : 1;                               //0x7e0
            ULONG SystemExtendedProcessInformation : 1;                       //0x7e0
            ULONG SystemSessionProcessInformation : 1;                        //0x7e0
            ULONG SystemMemoryTopologyInformation : 1;                        //0x7e0
            ULONG SystemMemoryChannelInformation : 1;                         //0x7e0
            ULONG SystemUnused : 1;                                           //0x7e0
            ULONG SystemPlatformBinaryInformation : 1;                        //0x7e0
            ULONG SystemFirmwareTableInformation : 1;                         //0x7e0
            ULONG SystemBootMetadataInformation : 1;                          //0x7e0
            ULONG SystemWheaIpmiHardwareInformation : 1;                      //0x7e0
            ULONG SystemSuperfetchPrefetch : 1;                               //0x7e0
            ULONG SystemSuperfetchPfnQuery : 1;                               //0x7e0
            ULONG SystemSuperfetchPrivSourceQuery : 1;                        //0x7e0
            ULONG SystemSuperfetchMemoryListQuery : 1;                        //0x7e0
            ULONG SystemSuperfetchMemoryRangesQuery : 1;                      //0x7e0
            ULONG SystemSuperfetchPfnSetPriority : 1;                         //0x7e0
            ULONG SystemSuperfetchMovePages : 1;                              //0x7e0
            ULONG SystemSuperfetchPfnSetPageHeat : 1;                         //0x7e0
            ULONG SysDbgGetTriageDump : 1;                                    //0x7e0
            ULONG SysDbgGetLiveKernelDump : 1;                                //0x7e0
            ULONG SyscallUsageValuesSpare : 5;                                //0x7e0
        } SyscallUsageValues;                                               //0x7e0
    };
    LONG SupervisorDeviceAsid;                                              //0x7e4
    VOID* SupervisorSvmData;                                                //0x7e8
    struct _PROCESS_NETWORK_COUNTERS* NetworkCounters;                      //0x7f0
    union _PROCESS_EXECUTION Execution;                                     //0x7f8
    VOID* ThreadIndexTable;                                                 //0x800
    struct _LIST_ENTRY FreezeWorkLinks;                                     //0x808
}EPROCESS, * PEPROCESS;

typedef NTSTATUS (NTAPI * PsLookupProcessByProcessId) (
    _In_  HANDLE    ProcessId,
    _Out_ PEPROCESS* Process
);

PEPROCESS GetEprocessByPid(IN ULONG_PTR pid, _Out_ int & errcode)
{
    PEPROCESS process;
    unsigned long processID = (unsigned long)pid;

    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    if (!hNtdll) { 
        errcode = 1;
        return NULL; 
    }

    PsLookupProcessByProcessId psLookupProcessByProcessId =
        (PsLookupProcessByProcessId)GetProcAddress(hNtdll, "PsLookupProcessByProcessId");
    if (!psLookupProcessByProcessId) {
        errcode = 2;
        return NULL;
    }



    if (psLookupProcessByProcessId(ULongToHandle(processID), &process) != 0)
    {
        //DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "Failed to get EPROCESS from PID\n");
        errcode = 3;
        return NULL;
    }

    return process;
}