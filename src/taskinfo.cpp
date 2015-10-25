/*
 * Traceshark - a visualizer for visualizing ftrace traces
 * Copyright (C) 2015  Viktor Rosendahl <viktor.rosendahl@gmail.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cputask.h"
#include "taskgraph.h"
#include "taskinfo.h"
#include "traceshark.h"
#include "qcustomplot/qcustomplot.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QString>

TaskInfo::TaskInfo(QWidget *parent):
	QWidget(parent), taskGraph(NULL)
{
	QHBoxLayout *layout  = new QHBoxLayout(this);
	QLabel *colonLabel = new QLabel(tr(":"));
	QPushButton *button = new QPushButton(tr("Add to legend"), this);

	nameLine = new QLineEdit(this);
	pidLine = new QLineEdit(this);


	nameLine->setReadOnly(true);
	pidLine->setReadOnly(true);

	layout->addWidget(nameLine);
	layout->addWidget(colonLabel);
	layout->addWidget(pidLine);
	layout->addWidget(button);

	tsconnect(button, clicked(), this, addClicked());
}

TaskInfo::~TaskInfo()
{
}


void TaskInfo::setTaskGraph(TaskGraph *graph)
{
	CPUTask *task = graph->getTask();
	if (task == NULL)
		return;
	QString nameStr = QString(task->name);
	QString pidStr = QString::number(task->pid);
	nameLine->setText(nameStr);
	pidLine->setText(pidStr);
	taskGraph = graph;
}

void TaskInfo::removeTaskGraph()
{
	taskGraph = NULL;
	nameLine->setText(tr(""));
	pidLine->setText(tr(""));
}

void TaskInfo::addClicked()
{
	QObject *obj;
	QCustomPlot *plot;
	if (taskGraph != NULL) {
		taskGraph->addToLegend();
		obj = taskGraph->parent();
		plot = qobject_cast<QCustomPlot *>(obj);
		if (plot != NULL)
			plot->replot();
	}
}

void TaskInfo::checkGraphSelection()
{
	if (taskGraph == NULL)
		return;
	if (taskGraph->selected())
		return;
	removeTaskGraph();
}
