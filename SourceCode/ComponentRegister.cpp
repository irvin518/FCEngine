#include "stdafx.h"
#include "Utility/BeatsUtility/ComponentSystem/ComponentPublic.h"
#include "Utility/BeatsUtility/ComponentSystem/Component/ComponentManager.h"
#include "Utility/BeatsUtility/Serializer.h"
#include "Utility/BeatsUtility/EnumStrGenerator.h"
#include "Resource/ResourcePathManager.h"
#include "Resource/Resource.h"
#include "Render/Model.h"
#include "Render/Skeleton.h"
#include "Render/RenderState.h"
#include "Render/Material.h"
#include "Render/TextureFragDescription.h"

#include "Render/RenderStateParam/RenderStateParamBase.h"
#include "Render/RenderStateParam/BoolRenderStateParam.h"
#include "Render/RenderStateParam/UintRenderStateParam.h"
#include "Render/RenderStateParam/IntRenderStateParam.h"
#include "Render/RenderStateParam/FloatRenderStateParam.h"
#include "Render/RenderStateParam/FunctionRenderStateParam.h"
#include "Render/RenderStateParam/BlendRenderStateParam.h"
#include "Render/RenderStateParam/BlendEquationRenderStateParam.h"
#include "Render/RenderStateParam/CullModeRenderStateParam.h"
#include "Render/RenderStateParam/ClockWiseRenderStateParam.h"
#include "Render/RenderStateParam/PolygonModeRenderStateParam.h"
#include "Render/RenderStateParam/ShadeModeRenderStateParam.h"
#include "Render/RenderStateParam/StencilRenderStateParam.h"

#include "GUI/Window/Window.h"
#include "GUI/Window/Button.h"
#include "GUI/Window/CheckBox.h"
#include "GUI/Window/ComboBox.h"
#include "GUI/Window/ListBox.h"
#include "GUI/Window/ListControl.h"
#include "GUI/Window/Progress.h"
#include "GUI/Window/Slider.h"
#include "GUI/Window/TextEdit.h"
#include "GUI/Window/TextLabel.h"
#include "GUI/Renderer/BaseRenderer.h"
#include "GUI/Renderer/RenderLayer.h"

#include "ParticlesSystem/ParticleSystem.h"
#include "ParticlesSystem/Emitter.h"
#include "ParticlesSystem/ParticleProperty.h"
#include "ParticlesSystem/BoxParticleEmitterEntity.h"
#include "ParticlesSystem/BallParticleEmitterEntity.h"
#include "ParticlesSystem/PointParticleEmitterEntity.h"
#include "ParticlesSystem/ConeParticleEmitterEntity.h"
#include "ParticlesSystem/ColorParticleAnimation.h"
#include "ParticlesSystem/RotateParticleAnimation.h"
#include "ParticlesSystem/ScaleParticleAnimation.h"
#include "ParticlesSystem/ParticleEmitterEntity.h"

#include "SkeletalAnimation/CAnimationModel.h"

//TankWar
#include "TankWar/TankComponent/Tank.h"
#include "TankWar/TankComponent/Pedestal.h"
#include "TankWar/TankComponent/Turret.h"
#include "TankWar/TankComponent/FireController.h"
#include "TankWar/TankComponent/Shield.h"
#include "TankWar/TankComponent/MainWeapon.h"
#include "TankWar/TankComponent/SubWeapon.h"
#include "TankWar/TankComponent/Driver.h"
#include "TankWar/TankComponent/Plug.h"
#include "TankWar/TankComponent/AdditiveAttribute.h"
#include "TankWar/Action/ActionBase.h"
#include "TankWar/Action/DropItem.h"
#include "TankWar/Action/Shot.h"
#include "TankWar/Item/Item.h"
#include "TankWar/Action/Buff.h"
#include "TankWar/Action/Die.h"
#include "TankWar/Action/Hide.h"
#include "TankWar/Action/Display.h"
#include "TankWar/Action/Move.h"
#include "TankWar/Action/Recover.h"
#include "TankWar/Action/Hurt.h"
#include "TankWar/Action/UseItem.h"
#include "TankWar/Action/UseSkill.h"
#include "TestComponent.h"
#include "Render/Animation.h"
#include "GUI/Font/Font.h"
#include "Render/Shader.h"
#include "Render/Skin.h"
#include "Render/Texture.h"
#include "Render/TextureAtlas.h"

void ForceReferenceSymbolForComponent()
{
    // When you are writing in a lib project, this cpp may not be referenced by the exe project.
    // so we add this empty function and make it extern in componentpublic.h, every cpp in the exe project,
    // which contain the componentpublic.h file will reference this symbol.
}

START_REGISTER_COMPONENT
REGISTER_ABSTRACT_COMPONENT(CResource)
REGISTER_COMPONENT(CModel, _T("模型"), _T("Resource"))
REGISTER_COMPONENT(CSkeleton, _T("骨骼"), _T("Resource"))
REGISTER_COMPONENT(CMaterial, _T("材质"), _T("Resource"))
REGISTER_COMPONENT(CAnimation, _T("动画"), _T("Resource"))
REGISTER_COMPONENT(FCGUI::CFont, _T("字体"), _T("Resource"))
REGISTER_COMPONENT(CShader, _T("着色器"), _T("Resource"))
REGISTER_COMPONENT(CSkin, _T("蒙皮"), _T("Resource"))
REGISTER_COMPONENT(CTexture, _T("纹理"), _T("Resource"))
REGISTER_COMPONENT(CTextureAtlas, _T("纹理集"), _T("Resource"))
REGISTER_COMPONENT(CTextureFragDescription, _T("TextureFragDescription"), _T("Resource"))

REGISTER_COMPONENT(CRenderState, _T("渲染状态"), _T("Render\\渲染状态"))

REGISTER_COMPONENT(_2DSkeletalAnimation::CAnimationModel,_T("CAnimationModel"),_T("2DSkeletalAnimation"));

REGISTER_COMPONENT(CBoolRenderStateParam, _T("CBoolRenderStateParam"), _T("Render\\渲染状态类型"))
REGISTER_COMPONENT(CUintRenderStateParam, _T("CUintRenderStateParam"), _T("Render\\渲染状态类型"))
REGISTER_COMPONENT(CIntRenderStateParam, _T("CIntRenderStateParam"), _T("Render\\渲染状态类型"))
REGISTER_COMPONENT(CFloatRenderStateParam, _T("CFloatRenderStateParam"), _T("Render\\渲染状态类型"))
REGISTER_COMPONENT(CFunctionRenderStateParam, _T("CFunctionRenderStateParam"), _T("Render\\渲染状态类型"))
REGISTER_COMPONENT(CBlendRenderStateParam, _T("CBlendRenderStateParam"), _T("Render\\渲染状态类型"))
REGISTER_COMPONENT(CBlendEquationRenderStateParam, _T("CBlendEquationParam"), _T("Render\\渲染状态类型"))
REGISTER_COMPONENT(CCullModeRenderStateParam, _T("CCullModeParam"), _T("Render\\渲染状态类型"))
REGISTER_COMPONENT(CClockWiseRenderStateParam, _T("CClockWiseParam"), _T("Render\\渲染状态类型"))
REGISTER_COMPONENT(CPolygonModeRenderStateParam, _T("CPolygonModeParam"), _T("Render\\渲染状态类型"))
REGISTER_COMPONENT(CShadeModeRenderStateParam, _T("CShadeModeParam"), _T("Render\\渲染状态类型"))
REGISTER_COMPONENT(CStencilRenderStateParam, _T("CStencilParam"), _T("Render\\渲染状态类型"))
REGISTER_ABSTRACT_COMPONENT(CRenderStateParamBase)

REGISTER_COMPONENT(FCGUI::CWindow, _T("Window"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CButton, _T("Button"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CCheckBox, _T("Check"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CListBox, _T("ListBox"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CComboBox, _T("ComboBox"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CProgress, _T("Progress"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CSlider, _T("Slider"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CTextLabel, _T("TextLabel"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CTextEdit, _T("TextEdit"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CListControl, _T("ListControl"), _T("GUI\\Window"))
REGISTER_COMPONENT(FCGUI::CRenderLayer, _T("RenderLayer"), _T("GUI\\Renderer"))
REGISTER_COMPONENT(FCGUI::CBaseRenderer, _T("BaseRenderer"), _T("GUI\\Renderer"))

REGISTER_COMPONENT(FCEngine::CParticleSystem, _T("粒子系统"), _T("粒子组件"))
REGISTER_COMPONENT(FCEngine::CParticleEmitter, _T("发射器"), _T("粒子组件") )
REGISTER_COMPONENT(FCEngine::CPointParticleEmitterEntity, _T("点发射器"), _T("粒子组件\\发射器类型") )
REGISTER_COMPONENT(FCEngine::CBoxParticleEmitterEntity, _T("盒子发射器"), _T("粒子组件\\发射器类型") )
REGISTER_COMPONENT(FCEngine::CBallParticleEmitterEntity, _T("球发射器"), _T("粒子组件\\发射器类型") )
REGISTER_COMPONENT(FCEngine::CConeParticleEmitterEntity, _T("锥发射器"), _T("粒子组件\\发射器类型") )
REGISTER_COMPONENT(FCEngine::CColorParticleAnimation, _T("颜色动画"), _T("粒子组件\\粒子动画类型") )
REGISTER_COMPONENT(FCEngine::CScaleParticleAnimation, _T("缩放动画"), _T("粒子组件\\粒子动画类型") )
REGISTER_COMPONENT(FCEngine::CRotateParticleAnimation, _T("旋转动画"), _T("粒子组件\\粒子动画类型") )
REGISTER_COMPONENT(FCEngine::CParticleProperty, _T("粒子属性"), _T("粒子组件") )
REGISTER_ABSTRACT_COMPONENT(FCEngine::CParticleEmitterEntityBase)

REGISTER_COMPONENT(CShaderUniform, _T("CShaderUniform"), _T("CShaderUniform"))
REGISTER_COMPONENT(CShaderUniformData, _T("CShaderUniformData"), _T("CShaderUniformData"))
REGISTER_ABSTRACT_COMPONENT(FCEngine::CParticleAnimationBase)

REGISTER_COMPONENT(CTank, _T("坦克"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CPedesta, _T("底座"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CTurret, _T("炮塔"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CFireController, _T("火力控制"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CShield, _T("护盾"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CMainWeapon, _T("主武器"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CSubWeapon, _T("副武器"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CAdditiveAttribute, _T("附加属性"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CPlugComponentBase, _T("坦克组件"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CPlug, _T("坦克组件库"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CDriver, _T("驾驶员"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CPlugConfigs, _T("坦克组件配置"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CTankConfigs, _T("坦克配置"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CPedestaConfigs, _T("底座配置"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CShieldConfigs, _T("护盾配置"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CMainWeaponConfigs, _T("主武器配置"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CSubWeaponConfigs, _T("副武器配置"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CTurretConfigs, _T("炮塔配置"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CFireControllerConfigs, _T("火控系统配置"),_T("TankWar\\TankComponent"))
REGISTER_COMPONENT(CDriverConfigs, _T("驾驶员配置"),_T("TankWar\\TankComponent"))

REGISTER_COMPONENT(CSkill, _T("技能"),_T("TankWar\\Skill"))
REGISTER_COMPONENT(CSkillConfigs, _T("技能配置"),_T("TankWar\\Skill"))
REGISTER_COMPONENT(CBullet, _T("子弹"),_T("TankWar\\Bullet"))
REGISTER_COMPONENT(CBulletConfigs, _T("子弹配置"),_T("TankWar\\Bullet"))
REGISTER_COMPONENT(CItem, _T("物品"),_T("TankWar\\Item"))
REGISTER_COMPONENT(CItemConfigs, _T("物品配置"),_T("TankWar\\Item"))
REGISTER_COMPONENT(CAuxiliaryAim, _T("辅助瞄准"),_T("TankWar\\Control"))
REGISTER_COMPONENT(CBuff, _T("增益效果"),_T("TankWar\\Action"))
REGISTER_COMPONENT(CDie, _T("死亡"),_T("TankWar\\Action"))
REGISTER_COMPONENT(CHurt, _T("受击"),_T("TankWar\\Action"))

REGISTER_COMPONENT(CMove, _T("移动"),_T("TankWar\\Action"))

REGISTER_COMPONENT(CRecover, _T("恢复"),_T("TankWar\\Action"))
REGISTER_COMPONENT(CDropItem, _T("掉落物品"),_T("TankWar\\Action"))
REGISTER_COMPONENT(CHide, _T("隐藏"),_T("TankWar\\Action"))
REGISTER_COMPONENT(CDisplay, _T("显示"),_T("TankWar\\Action"))

REGISTER_COMPONENT(CShot, _T("射击"),_T("TankWar\\Action"))
REGISTER_COMPONENT(CUseItem, _T("使用物品"),_T("TankWar\\Action"))
REGISTER_COMPONENT(CUseSkill, _T("使用技能"),_T("TankWar\\Action"))
REGISTER_ABSTRACT_COMPONENT(CActionBase)

REGISTER_COMPONENT(CTestComponent, _T("TestComponent"),_T("TestComponent"))
REGISTER_ABSTRACT_COMPONENT(CTestAbstractClass)
END_REGISTER_COMPONENT