#include "BBox.h"
#include <limits>

BBox::BBox() {
	minPt.setX(std::numeric_limits<float>::max());
	minPt.setY(std::numeric_limits<float>::max());
	maxPt.setX(-std::numeric_limits<float>::max());
	maxPt.setY(-std::numeric_limits<float>::max());
}

BBox::~BBox() {
}

/**
 * update the bounding box by combining aother bounding box.
 *
 * @param other aother bounding box
 */
void BBox::combineWithBBox(const BBox& other) {	
	minPt.setX(qMin(minPt.x(), other.minPt.x()));
	minPt.setY(qMin(minPt.y(), other.minPt.y()));

	maxPt.setX(qMax(maxPt.x(), other.maxPt.x()));
	maxPt.setY(qMax(maxPt.y(), other.maxPt.y()));
}

/**
 * update the bounding box by adding a new point.
 *
 * @param newPt new point
 */
void BBox::addPoint(const QVector2D& newPt) {
	minPt.setX(qMin(minPt.x(), newPt.x()));
	minPt.setY(qMin(minPt.y(), newPt.y()));

	maxPt.setX(qMax(maxPt.x(), newPt.x()));
	maxPt.setY(qMax(maxPt.y(), newPt.y()));
}

void BBox::translate(float x, float y) {
	minPt.setX(minPt.x() + x);
	minPt.setY(minPt.y() + y);
	maxPt.setX(maxPt.x() + x);
	maxPt.setY(maxPt.y() + y);
}

void BBox::resize(const QVector2D& pt) {
	switch (resizingType) {
	case RESIZING_TOP_LEFT:
		minPt.setX(pt.x());
		maxPt.setY(pt.y());
		break;
	case RESIZING_TOP_RIGHT:
		maxPt.setX(pt.x());
		maxPt.setY(pt.y());
		break;
	case RESIZING_BOTTOM_LEFT:
		minPt.setX(pt.x());
		minPt.setY(pt.y());
		break;
	case RESIZING_BOTTOM_RIGHT:
		maxPt.setX(pt.x());
		minPt.setY(pt.y());
		break;
	}
}

std::vector<QVector2D> BBox::polyline() const {
	std::vector<QVector2D> ret;

	ret.push_back(minPt);
	ret.push_back(QVector2D(maxPt.x(), minPt.y()));
	ret.push_back(maxPt);
	ret.push_back(QVector2D(minPt.x(), maxPt.y()));

	return ret;
}

bool BBox::hitTest(const QVector2D& pt) const {
	if (pt.x() < minPt.x() - dx() * 0.1f) return false;
	if (pt.y() < minPt.y() - dy() * 0.1f) return false;
	if (pt.x() > maxPt.x() + dx() * 0.1f) return false;
	if (pt.y() > maxPt.y() + dy() * 0.1f) return false;

	return true;
}

bool BBox::contains(const QVector2D &pt) const {
	if (pt.x() < minPt.x() || pt.y() < minPt.y()) return false;
	if (pt.x() > maxPt.x() || pt.y() > maxPt.y()) return false;

	return true;
}
