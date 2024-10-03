#---------- Start Project 3/7/2019 ---------------------------------------
#
#
# ملاحظة :
# عند معرفة زاوية بموقع البوينت
# pos -> lenth -> angel -> export newPos
# في
# angel -> atan2()
# يفرق الترتيب بين
# posX , posY
# أو
# posY , posX
# وأيضاً
# ترتيب ال
# sin , cos
# sin() تجيب طول ال y
# cos() تجيب طول ال x
#

# ===========================
#
# المؤشرات استخدمها للمتغيرات التي تستخدمها بعدما يخلّص الكلاس
# أو التي تأخذ قيم كثير وكبيرة حتى لا تُحمّل هلى الـ ستاك
#
# ===========================
# Project created by QtCreator 2019-07-03T17:31:34
# <MATH> : toSoon "rint" >> [ toBig "ceil" , toSmall "floor" ] ::  "nerrest"
# فلم الفضاء القديم لقطة الفنان العظيم
#
#-
# فكرة طلقات وقذائف
# الطلقات بالاجنحة وتتجه باتجاه الطائره الحالي
#  مقذوفات تحمل اثنين فقط وتقط للأسفل
# ولتذخير يتطلب المرور من خلال نقطة معينة
#

#-
# فكرة إعادة تموضع الماوس
# عند وصوله أحد طرفي النافذة إلى أقصى الرف الاخر
#
#

# ----- aXis -----
# Rot X : allowys World
# Rot Z : allowys Local
# Rot Y : ???


#------------------------------------------------
# لتحويل من راديان الى درجه
# القاعده العامه
# PI  --> 180ْ
# Rad --> x
# تعطينا قاعده
# #ex: Rad=2PI/8;
#
#      2*180
# x = ________
#        8
#
#
#
# لتحويل من درجه الى راديان
# PI --> 180ْ
# x  --> Beta
# تعطينا القاعده
#      Beta*PI
# x = ________
#       180
#
# // طريقة أخرى
# Radian To Degree
# radian * 180/float(M_PI)
#
# Degree To Radian
# angel * float(M_PI)/180
#
#
# // طريقة أخرى من مكتبة qMath.h
# # qRadiansToDegrees(Rad)
# # qDegreesToRadians(angel)
#



#
# فكرة ان اللف س=يبقى يزيد مع استمرارية الضغط
# وامكانية الاطلاق في آن واحد
#
#
# مقدمة للعبه تتجول الكاميرا في ميدان اللعبه
# بتاثيرات حركية متنوعه
# like this :
# https://youtu.be/4R7s0d68Wqw : 6 mintes
#
#
#
#
# ============
# MyLocal :
# r = sqrt(x^2+y^2+z^2)
# //V cosθ = (Y,r) --> Up&Down
# //H tanφ = (X,Z)      --> Left&Right
#
# Gloubal :
# Ex: P(2,3,4) //xyz
# r=√(2²+3²+4² )=√(4+9+16)=√29=5.385
# cosθ= z/r = 4/5.385 = 0.743Rad --> 42.03°
# tanφ= y/x = 3/2     = 1.5Rad   --> 56.31°
#
#
# Local:
# r=√(2²+3²+4² )=√(4+9+16)=√29=5.385
# cosθ= y/r = 3/5.385 = 0.5571  --> 31.935844437
# tanφ= z/x = 4/2 = 2           --> 114.649
#
#========================================
# من الافكار طريقة تحصيل النقاط
# عندما يكون الكسب الأعلى عند الاستهداف وانت ملتف للجوانب أو للأعلى والأسفل
# أي وأنت ضاغط على الجير
# تكون الطلقه تتحرك في مسار منحني   (
#
#
#======================
# تأثيرات :
# عند السقوط على الارض نعمل التفاف للطائرة حول "واي" مثلاً
# ونجعل الكاميرا تبتعد قليلاً
#
# ===========
# أهمية وضع كلمة this
# حتى يحذف المؤشر عند حذف الاب
#
#
# ===========================================
# ديناميكية الكاميرا مع تحكم طفيف للنظر
# مثلاً
# التحكم :
# عند الضغط إل2 يكون التحكم مفعل من خلال القير-ر
# وعند إفلات ضغط إل2 يلغى التحكم من خلال القير-ر و يعاد ضبط إعداد الكاميرا
#
# الديناميكية :
# أيضاً الديناميكية تكون مع تحرك الطائرة
#
#
# التصادم يتخذ حالة الدوران للجسم كالطائرة مثلاً وطو الجناحي مع قصر الطول عمودياً
# تختلف القياسات مع الدوران
#
#
# ملاحظة
# لايجب تزويد المتغير
# speedPlane
# عن واحد لأنه سيؤثر على تصادم الطائره مع الارض
#
# في الخيوط لا تتعامل مع المرجعيات والمؤشرات
# وإنما بالإشارات والفتحات
#
#
# +++++++++++++++++++++++
# مهمه :
# تفعيل إحتراق المنزل
# تفعيل تحطم الطائرة عند الإصطدام بالأرض أو المنازل ويكون هناك تحذير عند الإقتراب
#
#
# الخطّة التالية
# تمكين الهبوط باحتراف على مسار محدد
# أولاً النزول لمستوى منخفظ مع سرعه منخفظه
# ضغط آر2 كبريك مع تزايد روتاشن الإكس للأمام
# حيث إذا زاد كثيراً تتحطم الطائرة
#
# عندها يكون سيارة جاهزة للركوب
# تفعيل إنتقال الكاميرا للسيارة والسير على مستوى الأرض
#
# وتفعيل السلاح
#
# ---- مود آخر -----
# تمكين طائرات معادية
#
# ---- مود الضباب ----
#
#
# فكرة كشاف رادار
# مثلاً الطائرة العدو لها كشاف مقدمتها بانفراجه زاوية 45 مثلاً
# مثل الكشاف الذي برأس برج انسبشن
# أذا كنت في مدى هذه الزاوي سيكتشفني
# إلا إذا كنت خلف جدار او منزل
#
#
#

#   QTimer::singleShot(5000, this, [=](){
#       qDebug("Cleared");
#   });
#-------------------------------------------------
# [ المودل . . العنصر والعنصر اﻵخر التغييرات المرتبطه  ]
#  الشرح
#  عشان يدور العنصر حول عنصر اخر بتناغم
#  نعطيه نفس خصائص العنصر الذي يدور حوله
#  موقع ودوران وكأنه نسخه منه
#  يكون في هذه الحاله مندمج مع العنصر
#  إذا أردنا أن نغير موقع العنصر أو دورانه بشكل مستقل حول نفسه
#  نصنع ماتريكس آخر من خلاله نضع التعديلات
#  في هذه الحاله يكون كما حركت الذراع
#  متناغمه مع الساعد ومرتبطة ويمكن دورانها حول نفسها كالمرفق
#
#  * ترتيب الضرب مهم
#  أولاً نضع الماتريكس للمودل
#  ثم نضع الماتريكس للإنتيتي والذي يكون مندمج مع العنصر المرتبط به
#   ثم نضع الماتريكس التي بها خصائص الدوران حول نفسها
# -------------------------------------------------------

QT       += core gui gamepad concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = example
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    basemodel.cpp \
    collision.cpp \
    controllercollision.cpp \
    controllerfire.cpp \
    controllerrocket.cpp \
    controllerthrower.cpp \
#    controllerwater.cpp \
        entity.cpp \
        entityemitterfire.cpp \
#        entityparticle.cpp \
        entityparticlefire.cpp \
        entityrocket.cpp \
    entitywedge.cpp \
        main.cpp \
        mainwidget.cpp \
    modelcar.cpp \
    modelerth.cpp \
    modelgun.cpp \
    modelgunthrower.cpp \
    modelhome.cpp \
#    modelhumanenemy.cpp \
    modelhumanh.cpp \
    modelplane.cpp \
#    threadfire.cpp \
    modelrobot.cpp \
    modelwater.cpp \
    testwedget.cpp \
    vaoentity.cpp \
    watersupport.cpp \
        widget.cpp \
        gamePS.cpp \
        PerlinNoise.cpp \
        assimplib.cpp \
        vaocolladaassimp.cpp

HEADERS += \
    basemodel.h \
    collision.h \
    controllercollision.h \
    controllerfire.h \
    controllerrocket.h \
    controllerthrower.h \
#    controllerwater.h \
        entity.h \
        entityemitterfire.h \
#        entityparticle.h \
        entityparticlefire.h \
        entityrocket.h \
    entitywedge.h \
        mainwidget.h \
    modelcar.h \
    modelerth.h \
    modelgun.h \
    modelgunthrower.h \
    modelhome.h \
#    modelhumanenemy.h \
    modelhumanh.h \
    modelplane.h \
#    threadfire.h \
    modelrobot.h \
    modelwater.h \
    testwedget.h \
    vaoentity.h \
    watersupport.h \
        widget.h \
        gamePS.h \
        PerlinNoise.h\
        assimplib.h \
        vaocolladaassimp.h

FORMS += \
        widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    shaders.qrc

LIBS += -lassimp
