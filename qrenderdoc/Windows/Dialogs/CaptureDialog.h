/******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Baldur Karlsson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#pragma once

#include <QFrame>
#include <functional>
#include "Code/CaptureContext.h"

namespace Ui
{
class CaptureDialog;
}

class QStandardItemModel;
class LiveCapture;

class CaptureDialog : public QFrame, public ICaptureDialog
{
  Q_OBJECT

public:
  typedef std::function<void(const QString &exe, const QString &workingDir, const QString &cmdLine,
                             const QList<EnvironmentModification> &env, CaptureOptions opts,
                             std::function<void(LiveCapture *)> callback)>
      OnCaptureMethod;
  typedef std::function<void(uint32_t PID, const QList<EnvironmentModification> &env, const QString &name,
                             CaptureOptions opts, std::function<void(LiveCapture *)> callback)>
      OnInjectMethod;

  explicit CaptureDialog(ICaptureContext &ctx, OnCaptureMethod captureCallback,
                         OnInjectMethod injectCallback, QWidget *parent = 0);
  ~CaptureDialog();

  // ICaptureDialog
  QWidget *Widget() override { return this; }
  bool IsInjectMode() override { return m_Inject; }
  void SetInjectMode(bool inject) override;

  void SetExecutableFilename(const QString &filename) override;
  void SetWorkingDirectory(const QString &dir) override;
  void SetCommandLine(const QString &cmd) override;
  void SetEnvironmentModifications(const QList<EnvironmentModification> &modifications) override;

  void SetSettings(CaptureSettings settings) override;
  CaptureSettings Settings() override;

  void TriggerCapture() override;

  void LoadSettings(QString filename) override;
  void SaveSettings(QString filename) override;
  void UpdateGlobalHook() override;

private slots:
  // automatic slots
  void on_exePathBrowse_clicked();
  void on_exePath_textChanged(const QString &arg1);
  void on_workDirBrowse_clicked();
  void on_envVarEdit_clicked();

  void on_processFilter_textChanged(const QString &arg1);
  void on_processRefesh_clicked();

  void on_saveSettings_clicked();
  void on_loadSettings_clicked();

  void on_launch_clicked();
  void on_close_clicked();

  void on_toggleGlobal_clicked();

  void on_vulkanLayerWarn_clicked();

  void on_CaptureCallstacks_toggled(bool checked);

private:
  Ui::CaptureDialog *ui;
  ICaptureContext &m_Ctx;

  QStandardItemModel *m_ProcessModel;

  OnCaptureMethod m_CaptureCallback;
  OnInjectMethod m_InjectCallback;

  QList<EnvironmentModification> m_EnvModifications;
  bool m_Inject;
  void fillProcessList();
};
