//
// Created by softboy on 18/05/2017.
//
#include "zapi/Global.h"
#include "zapi/lang/Variant.h"
#include "zapi/vm/Zval.h"
#include "zapi/lang/FatalError.h"
#include "zapi/lang/Type.h"

#include "php/sapi/embed/php_embed.h"
#include "php/Zend/zend_types.h"

#include "gtest/gtest.h"

#include <iostream>
#include <limits>
#include <string>
#include <cstring>

using zapi::lang::Variant;
using zapi::lang::FatalError;
using zapi::lang::Type;
using zapi::vm::Zval;

namespace
{

class VariantTest : public ::testing::Test
{
   
};

}

TEST_F(VariantTest, testConstructor)
{
   Variant nullvar(nullptr);
   const Zval &nullval = nullvar.getZval();
   ASSERT_TRUE(Z_TYPE_P(nullval) == IS_NULL);
   int16_t intValue = 12;
   Variant var(intValue);
   const Zval &val = var.getZval();
   ASSERT_EQ(Z_LVAL_P(val), intValue);
   Variant var1(std::numeric_limits<int16_t>::min());
   const Zval &val1 = var1.getZval();
   ASSERT_EQ(Z_LVAL_P(val1), std::numeric_limits<int16_t>::min());
   ASSERT_EQ(Z_TYPE_P(var1.getZval()), IS_LONG);
   Variant var2(std::numeric_limits<int16_t>::max());
   ASSERT_EQ(Z_LVAL_P(var2.getZval()), std::numeric_limits<int16_t>::max());
   ASSERT_EQ(Z_TYPE_P(var2.getZval()), IS_LONG);
   Variant var3(std::numeric_limits<int32_t>::min());
   ASSERT_EQ(Z_LVAL_P(var3.getZval()), std::numeric_limits<int32_t>::min());
   Variant var4(std::numeric_limits<int32_t>::max());
   ASSERT_EQ(Z_LVAL_P(var4.getZval()), std::numeric_limits<int32_t>::max());
   ASSERT_EQ(Z_TYPE_P(var4.getZval()), IS_LONG);
   Variant var5(std::numeric_limits<int64_t>::min());
   ASSERT_EQ(Z_LVAL_P(var5.getZval()), std::numeric_limits<int64_t>::min());
   ASSERT_EQ(Z_TYPE_P(var5.getZval()), IS_LONG);
   Variant var6(std::numeric_limits<int64_t>::max());
   ASSERT_EQ(Z_LVAL_P(var6.getZval()), std::numeric_limits<int64_t>::max());
   ASSERT_EQ(Z_TYPE_P(var6.getZval()), IS_LONG);
   // test boolean
   Variant trueVariant(true);
   ASSERT_EQ(Z_TYPE_P(trueVariant.getZval()), IS_TRUE);
   Variant falseVariant(false);
   ASSERT_EQ(Z_TYPE_P(falseVariant.getZval()), IS_FALSE);
   // char constructor test
   Variant charVariant('a');
   char *retCharArr = Z_STRVAL_P(charVariant.getZval());
   ASSERT_EQ(retCharArr[0], 'a');
   ASSERT_EQ(Z_STRLEN_P(charVariant.getZval()), 1);
   // test constructor(const std::string &value)
   std::string zapiStr("zapi");
   Variant zapiStrVar(zapiStr);
   char *zapiStrPtr = Z_STRVAL_P(zapiStrVar.getZval());
   ASSERT_EQ(Z_STRLEN_P(zapiStrVar.getZval()), 4);
   ASSERT_EQ(std::strncmp(zapiStrPtr, "zapi", 4), 0);
   ASSERT_EQ(Z_TYPE_P(zapiStrVar.getZval()), IS_STRING);
   // test constructor(const char *value, int length = -1)
   Variant sizeStr1("hello zapi!", -1);
   ASSERT_EQ(Z_STRLEN_P(sizeStr1.getZval()), 11);
   ASSERT_EQ(std::strncmp(Z_STRVAL_P(sizeStr1.getZval()), "hello zapi!", 11), 0);
   ASSERT_EQ(Z_TYPE_P(sizeStr1.getZval()), IS_STRING);
   Variant sizeStr2("hello zapi!", 5);
   ASSERT_EQ(Z_STRLEN_P(sizeStr2.getZval()), 5);
   ASSERT_EQ(std::strncmp(Z_STRVAL_P(sizeStr2.getZval()), "hello", 5), 0);
   ASSERT_EQ(Z_TYPE_P(sizeStr2.getZval()), IS_STRING);
   Variant sizeStr3("hello zapi!", 20);
   ASSERT_EQ(Z_STRLEN_P(sizeStr3.getZval()), 20);
   ASSERT_EQ(std::strncmp(Z_STRVAL_P(sizeStr3.getZval()), "hello zapi!\0\0\0\0\0\0\0\0\0", 20), 0);
   ASSERT_EQ(Z_TYPE_P(sizeStr3.getZval()), IS_STRING);
   Variant sizeStr4("hello zapi!");
   ASSERT_EQ(Z_STRLEN_P(sizeStr4.getZval()), 11);
   ASSERT_EQ(std::strncmp(Z_STRVAL_P(sizeStr4.getZval()), "hello zapi!", 11), 0);
   ASSERT_EQ(Z_TYPE_P(sizeStr4.getZval()), IS_STRING);
   // test constructor(double value)
   double dvalue = 0.12;
   Variant doubleVar(dvalue);
   ASSERT_EQ(Z_TYPE_P(doubleVar.getZval()), IS_DOUBLE);
   ASSERT_EQ(Z_DVAL_P(doubleVar.getZval()), 0.12);
   double dvalue1 = -0.12;
   Variant doubleVar1(dvalue1);
   ASSERT_EQ(Z_TYPE_P(doubleVar1.getZval()), IS_DOUBLE);
   ASSERT_EQ(Z_DVAL_P(doubleVar1.getZval()), -0.12);
   // test type constructor
   ASSERT_THROW({Variant undefinedVar(Type::Undefined);}, FatalError);
   ASSERT_THROW({Variant resourceVar(Type::Resource);}, FatalError);
   ASSERT_THROW({Variant constVar(Type::Constant);}, FatalError);
   ASSERT_THROW({Variant constAstVar(Type::ConstantAST);}, FatalError);
   ASSERT_THROW({Variant callableVar(Type::Callable);}, FatalError);
   ASSERT_THROW({Variant refVar(Type::Reference);}, FatalError);
   Variant nullTypeVar(Type::Null);
   ASSERT_EQ(Z_TYPE_P(nullTypeVar.getZval()), IS_NULL);
   Variant falseTypeVar(Type::False);
   ASSERT_EQ(Z_TYPE_P(falseTypeVar.getZval()), IS_FALSE);
   Variant trueTypeVar(Type::True);
   ASSERT_EQ(Z_TYPE_P(trueTypeVar.getZval()), IS_TRUE);
   Variant longTypeVar(Type::Long);
   ASSERT_EQ(Z_TYPE_P(longTypeVar.getZval()), IS_LONG);
   Variant doubleTypeVar(Type::Double);
   ASSERT_EQ(Z_TYPE_P(doubleTypeVar.getZval()), IS_DOUBLE);
   Variant stringTypeVar(Type::String);
   ASSERT_EQ(Z_TYPE_P(stringTypeVar.getZval()), IS_STRING);
   Variant arrayTypeVar(Type::Array);
   ASSERT_EQ(Z_TYPE_P(arrayTypeVar.getZval()), IS_ARRAY);
   Variant objectTypeVar(Type::Object);
   ASSERT_EQ(Z_TYPE_P(objectTypeVar.getZval()), IS_OBJECT);
   
   // test construct from other _zval_struct *
   zval rawVar;
   ZVAL_LONG(&rawVar, 2017);
   Variant constructFromRaw(&rawVar);
   ASSERT_EQ(Z_LVAL_P(constructFromRaw.getZval()), 2017);
   ASSERT_EQ(Z_TYPE_P(constructFromRaw.getZval()), IS_LONG);
   ZVAL_LONG(&rawVar, 2018);
   ASSERT_EQ(Z_LVAL_P(constructFromRaw.getZval()), 2017);
   ASSERT_EQ(Z_LVAL_P(&rawVar), 2018);
   zval rawVar1;
   ZVAL_LONG(&rawVar1, 2017);
   Variant constructFromRawRef(&rawVar1, true);
   ASSERT_EQ(Z_TYPE_P(constructFromRawRef.getZval()), IS_LONG);
   ASSERT_EQ(Z_LVAL_P(constructFromRawRef.getZval()), 2017);
   // test copy constructor
   {
      Variant orig(1234);
      Variant me(orig);
      ASSERT_EQ(Z_TYPE_P(me.getZval()), Z_TYPE_P(orig.getZval()));
      ASSERT_EQ(Z_LVAL_P(me.getZval()), Z_LVAL_P(orig.getZval()));
      Variant origStr("zapi", 4);
      Variant meStr(origStr);
      ASSERT_EQ(Z_TYPE_P(meStr.getZval()), Z_TYPE_P(origStr.getZval()));
      ASSERT_EQ(Z_LVAL_P(meStr.getZval()), Z_LVAL_P(origStr.getZval()));
   }
   {
      // test move constructor 
      Variant orig("zapi", 4);
      Variant me(std::move(orig));
      ASSERT_EQ(Z_TYPE_P(orig.getZval()), IS_UNDEF);
      ASSERT_EQ(Z_TYPE_P(me.getZval()), IS_STRING);
      ASSERT_EQ(std::strncmp(Z_STRVAL_P(me.getZval()), "zapi", 4), 0);
   }
}

TEST_F(VariantTest, testAssignOperator)
{
   {
      Variant orig(2018);
      Variant me("hello", 5);
      me = orig;
      ASSERT_EQ(Z_TYPE_P(orig.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(me.getZval()), 2018);
   }
   {
      // test number assign operators
      Variant var(1);
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), 1);
      var = static_cast<int16_t>(123);
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), 123);
      var = std::numeric_limits<int16_t>::min();
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), std::numeric_limits<int16_t>::min());
      var = std::numeric_limits<int16_t>::max();
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), std::numeric_limits<int16_t>::max());
      var = static_cast<int32_t>(1232);
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), 1232);
      var = std::numeric_limits<int32_t>::min();
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), std::numeric_limits<int32_t>::min());
      var = std::numeric_limits<int32_t>::max();
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), std::numeric_limits<int32_t>::max());
      var = static_cast<int64_t>(2018);
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), 2018);
      var = std::numeric_limits<int64_t>::min();
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), std::numeric_limits<int64_t>::min());
      var = std::numeric_limits<int64_t>::max();
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_LONG);
      ASSERT_EQ(Z_LVAL_P(var.getZval()), std::numeric_limits<int64_t>::max());
      // test double assign operators
      var = 0.12;
      ASSERT_EQ(Z_TYPE_P(var.getZval()), IS_DOUBLE);
      ASSERT_EQ(Z_DVAL_P(var.getZval()), 0.12);
   }
   {
      // test boolean assign operators
      Variant booleanVar;
      ASSERT_EQ(Z_TYPE_P(booleanVar.getZval()), IS_NULL);
      booleanVar = true;
      ASSERT_EQ(Z_TYPE_P(booleanVar.getZval()), IS_TRUE);
      booleanVar = false;
      ASSERT_EQ(Z_TYPE_P(booleanVar.getZval()), IS_FALSE);
   }
   {
      // string assign operators
      Variant strVar;
      ASSERT_EQ(Z_TYPE_P(strVar.getZval()), IS_NULL);
      strVar = std::string("zapi");
      ASSERT_EQ(Z_TYPE_P(strVar.getZval()), IS_STRING);
      ASSERT_EQ(std::strncmp(Z_STRVAL_P(strVar.getZval()), "zapi", 4), 0);
      strVar = 'a';
      ASSERT_EQ(Z_TYPE_P(strVar.getZval()), IS_STRING);
      ASSERT_EQ(std::strncmp(Z_STRVAL_P(strVar.getZval()), "a", 1), 0);
      strVar = "hello zapi";
      ASSERT_EQ(Z_TYPE_P(strVar.getZval()), IS_STRING);
      ASSERT_EQ(std::strncmp(Z_STRVAL_P(strVar.getZval()), "hello zapi", 10), 0);
   }
}

int main(int argc, char **argv)
{
   int retCode = 0;
   PHP_EMBED_START_BLOCK(argc,argv);
   ::testing::InitGoogleTest(&argc, argv);
   retCode = RUN_ALL_TESTS();
   PHP_EMBED_END_BLOCK();
   return retCode;
}

