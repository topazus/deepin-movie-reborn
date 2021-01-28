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
#include "slider.h"

#include <DThemeManager>
#include <DApplication>
#include <QProcess>

#define TOOLBOX_TOP_EXTENT 12
DWIDGET_USE_NAMESPACE

namespace dmr {
/**
 * @brief DMRSlider::DMRSlider 构造函数
 * @param parent 父窗口
 */
DMRSlider::DMRSlider(QWidget *parent): DSlider(Qt::Horizontal, parent)
{
    initMember();
    slider()->setTracking(false);
    slider()->setMouseTracking(true);
    setMouseTracking(true);
}

void DMRSlider::setEnableIndication(bool on)
{
    if (m_bIndicatorEnabled != on) {
        m_bIndicatorEnabled = on;
        update();
    }
}
/**
 * @brief ~DMRSlider 析构函数
 */
DMRSlider::~DMRSlider()
{
}
/**
 * @brief mouseReleaseEvent 鼠标释放事件函数
 * @param pMouseEvent
 */
void DMRSlider::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_bDown) {
        m_bDown = false;
        QWidget::mouseReleaseEvent(e);
    }
}
/**
 * @brief position2progress 像素点到进度条位置转换
 * @param p 像素点
 * @return 进度条位置
 */
int DMRSlider::position2progress(const QPoint &p)
{
    qreal total = (maximum() - minimum());

    if (orientation() == Qt::Horizontal) {
        qreal span = static_cast<qreal>(total) / contentsRect().width();
        return static_cast<int>(span * (p.x()) + minimum());
    } else {
        qreal span = static_cast<qreal>(total) / contentsRect().height();
        return static_cast<int>(span * (height() - p.y()) + minimum());
    }
}
/**
 * @brief mousePressEvent 鼠标按下事件函数
 * @param pMouseEvent 鼠标按下事件
 */
void DMRSlider::mousePressEvent(QMouseEvent *e)
{
    QProcessEnvironment systemEnv = QProcessEnvironment::systemEnvironment();
    QString XDG_SESSION_TYPE = systemEnv.value(QStringLiteral("XDG_SESSION_TYPE"));
    QString WAYLAND_DISPLAY = systemEnv.value(QStringLiteral("WAYLAND_DISPLAY"));

    if (XDG_SESSION_TYPE == QLatin1String("wayland") ||
            WAYLAND_DISPLAY.contains(QLatin1String("wayland"), Qt::CaseInsensitive)) {
        return ;
    }

    if (e->buttons() == Qt::LeftButton && isEnabled()) {
        QWidget::mousePressEvent(e);

        int v = position2progress(e->pos());;
        //wayland 此处注释
        //slider()->setSliderPosition(v);
        emit sliderMoved(v);
        m_bDown = true;
    }
}
/**
 * @brief mouseMoveEvent 鼠标移动事件函数
 * @param pMouseEvent 鼠标事件
 */
void DMRSlider::mouseMoveEvent(QMouseEvent *e)
{
    if (!isEnabled()) return;

    int nValue = position2progress(e->pos());
    if (m_bDown) {
        if (m_bShowIndicator) {
            m_indicatorPos = {e->x(), pos().y() + TOOLBOX_TOP_EXTENT - 4};
            update();
        }
    } else {
        // a mouse enter from previewer happens
        if (m_bIndicatorEnabled && !property("Hover").toBool()) {
            setProperty("Hover", "true");
            m_bShowIndicator = true;
            update();
        }
        emit enter();

        if (m_nLastHoverValue != nValue) {
            if (m_bShowIndicator) {
                m_indicatorPos = {e->x(), pos().y() + TOOLBOX_TOP_EXTENT - 4};
                update();
            }
            emit hoverChanged(nValue);
        }
        m_nLastHoverValue = nValue;
    }
    e->accept();
}
/**
 * @brief leaveEvent 鼠标离开事件函数
 * @param pEvent 事件
 */
void DMRSlider::leaveEvent(QEvent *pEvent)
{
    if (m_bIndicatorEnabled) {
        m_bShowIndicator = false;
        update();
    }

    //HACK: workaround problem that preview will make slider leave
    QPoint pos = mapFromGlobal(QCursor::pos());
    if (pos.y() > 0 && pos.y() < 6) {
        // preview may popup
        return;
    }

    m_nLastHoverValue = 0;
    if (m_bDown) m_bDown = false;

    emit leave();
    if (pEvent) pEvent->accept();
}
/**
 * @brief forceLeave 离开范围调用
 */
void DMRSlider::forceLeave()
{
    leaveEvent(nullptr);
}
/**
 * @brief enterEvent 鼠标进入事件函数
 * @param pEvent 事件
 */
void DMRSlider::enterEvent(QEvent *pEvent)
{
    if (m_bIndicatorEnabled) {
        if (property("Hover") != "true") {
            setProperty("Hover", "true");
            m_bShowIndicator = true;
            update();
        }
    }
    emit enter();
    pEvent->accept();
}
/**
 * @brief wheelEvent 鼠标滚轮事件函数
 * @param pWheelEvent 鼠标滚轮事件
 */
void DMRSlider::wheelEvent(QWheelEvent *pWheelEvent)
{
    if (pWheelEvent->buttons() == Qt::MiddleButton && pWheelEvent->modifiers() == Qt::NoModifier) {
        qInfo() << "angleDelta" << pWheelEvent->angleDelta();
    }
    pWheelEvent->accept();
}
/**
 * @brief paintEvent 重载绘制事件函数
 * @param pPaintEvent 绘制事件
 */
void DMRSlider::paintEvent(QPaintEvent *pPaintEvent)
{
    QWidget::paintEvent(pPaintEvent);
}

void DMRSlider::initMember()
{
    m_bDown = false;
    m_bIndicatorEnabled = false;
    m_bShowIndicator = false;
    m_nLastHoverValue = 0;
    m_indicatorPos = {0, 0};
}

}

