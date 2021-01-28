/* 
 * (c) 2017, Deepin Technology Co., Ltd. <support@deepin.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * is provided AS IS, WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, and
 * NON-INFRINGEMENT.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/**
 * @file 这个文件是播放进度条相关
 */
#ifndef _DMR_SLIDER_H
#define _DMR_SLIDER_H 

#include <QtWidgets>
#include <DSlider>
#include <DImageButton>
DWIDGET_USE_NAMESPACE
namespace dmr {
/**
 * @brief The DMRSlider class
 * 实现播放进度条
 */
class DMRSlider: public DSlider {
    Q_OBJECT
public:
    /**
     * @brief DMRSlider 构造函数
     * @param parent 父窗口
     */
    explicit DMRSlider(QWidget *parent = 0);
    /**
     * @brief ~DMRSlider 析构函数
     */
    virtual ~DMRSlider();
    /**
     * @brief setEnableIndication
     * @param on
     */
    void setEnableIndication(bool on);

    //workaround
    /**
     * @brief forceLeave 离开范围调用
     */
    void forceLeave();

signals:
    /**
     * @brief hoverChanged 悬停变更信号
     */
    void hoverChanged(int);
    /**
     * @brief leave 鼠标离开信号
     */
    void leave();
    /**
     * @brief enter 鼠标进入信号
     */
    void enter();

protected:
    /**
     * @brief onValueChanged 进度条值改变槽函数
     * @param v 值
     */
    void onValueChanged(const QVariant& v);
    /**
     * @brief onAnimationStopped 动画结束槽函数
     */
    void onAnimationStopped();

protected:
    /**
     * @brief mouseReleaseEvent 鼠标释放事件函数
     * @param pMouseEvent
     */
    void mouseReleaseEvent(QMouseEvent *pMouseEvent) override;
    /**
     * @brief mousePressEvent 鼠标按下事件函数
     * @param pMouseEvent 鼠标按下事件
     */
    void mousePressEvent(QMouseEvent *pMouseEvent) override;
    /**
     * @brief mouseMoveEvent 鼠标移动事件函数
     * @param pMouseEvent 鼠标事件
     */
    void mouseMoveEvent(QMouseEvent *pMouseEvent) override;
    /**
     * @brief leaveEvent 鼠标离开事件函数
     * @param pEvent 事件
     */
    void leaveEvent(QEvent *pEvent) override;
    /**
     * @brief enterEvent 鼠标进入事件函数
     * @param pEvent 事件
     */
    void enterEvent(QEvent *pEvent) override;
    /**
     * @brief wheelEvent 鼠标滚轮事件函数
     * @param pWheelEvent 鼠标滚轮事件
     */
    void wheelEvent(QWheelEvent *pWheelEvent) override;
    /**
     * @brief paintEvent 重载绘制事件函数
     * @param pPaintEvent 绘制事件
     */
    void paintEvent(QPaintEvent *pPaintEvent) override;

    /**
     * @brief initMember 初始化成员变量
     */
    void initMember();

private:
    /**
     * @brief position2progress 像素点到进度条位置转换
     * @param p 像素点
     * @return 进度条位置
     */
    int position2progress(const QPoint& p);
    /**
     * @brief startAnimation
     * @param reverse
     */
    void startAnimation(bool reverse);

    bool m_bDown;                             ///鼠标是否按下标志位
    bool m_bIndicatorEnabled;                 ///进入后设置显示状态
    bool m_bShowIndicator;                    ///是否显示当前位置
    int m_nLastHoverValue;                    ///上次悬停的位置
    QPoint m_indicatorPos;                    ///鼠标悬停的位置
    QColor m_indicatorColor;                  ///鼠标悬停活动色
};

}

#endif /* ifndef _DMR_SLIDER_H */
