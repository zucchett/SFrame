// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdIDMSearches_Dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "include/DMSearches.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_DMAnalysis(void *p = 0);
   static void *newArray_DMAnalysis(Long_t size, void *p);
   static void delete_DMAnalysis(void *p);
   static void deleteArray_DMAnalysis(void *p);
   static void destruct_DMAnalysis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DMAnalysis*)
   {
      ::DMAnalysis *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DMAnalysis >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DMAnalysis", ::DMAnalysis::Class_Version(), "include/DMSearches.h", 60,
                  typeid(::DMAnalysis), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DMAnalysis::Dictionary, isa_proxy, 4,
                  sizeof(::DMAnalysis) );
      instance.SetNew(&new_DMAnalysis);
      instance.SetNewArray(&newArray_DMAnalysis);
      instance.SetDelete(&delete_DMAnalysis);
      instance.SetDeleteArray(&deleteArray_DMAnalysis);
      instance.SetDestructor(&destruct_DMAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DMAnalysis*)
   {
      return GenerateInitInstanceLocal((::DMAnalysis*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DMAnalysis*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr DMAnalysis::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DMAnalysis::Class_Name()
{
   return "DMAnalysis";
}

//______________________________________________________________________________
const char *DMAnalysis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DMAnalysis*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DMAnalysis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DMAnalysis*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DMAnalysis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DMAnalysis*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DMAnalysis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DMAnalysis*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void DMAnalysis::Streamer(TBuffer &R__b)
{
   // Stream an object of class DMAnalysis.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DMAnalysis::Class(),this);
   } else {
      R__b.WriteClassBuffer(DMAnalysis::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DMAnalysis(void *p) {
      return  p ? new(p) ::DMAnalysis : new ::DMAnalysis;
   }
   static void *newArray_DMAnalysis(Long_t nElements, void *p) {
      return p ? new(p) ::DMAnalysis[nElements] : new ::DMAnalysis[nElements];
   }
   // Wrapper around operator delete
   static void delete_DMAnalysis(void *p) {
      delete ((::DMAnalysis*)p);
   }
   static void deleteArray_DMAnalysis(void *p) {
      delete [] ((::DMAnalysis*)p);
   }
   static void destruct_DMAnalysis(void *p) {
      typedef ::DMAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DMAnalysis

namespace {
  void TriggerDictionaryInitialization_DMSearches_Dict_Impl() {
    static const char* headers[] = {
"include/DMSearches.h",
0
    };
    static const char* includePaths[] = {
"/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/SFrame",
"./",
"/cvmfs/cms.cern.ch/slc6_amd64_gcc530/lcg/root/6.06.00-ikhhed6/include",
"/mnt/t3nfs01/data01/shome/zucchett/SFrameAnalysis/DM/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "DMSearches_Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$include/DMSearches.h")))  DMAnalysis;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "DMSearches_Dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "include/DMSearches.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"DMAnalysis", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("DMSearches_Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_DMSearches_Dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_DMSearches_Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_DMSearches_Dict() {
  TriggerDictionaryInitialization_DMSearches_Dict_Impl();
}
