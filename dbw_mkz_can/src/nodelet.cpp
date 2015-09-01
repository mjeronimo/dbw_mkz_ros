#include <pluginlib/class_list_macros.h>
#include <nodelet/nodelet.h>

#include "DbwNode.h"

namespace dbw_mkz_can
{

class DbwNodelet : public nodelet::Nodelet
{
public:
  DbwNodelet()
  {
  }
  ~DbwNodelet()
  {
  }

  void onInit(void)
  {
    node_.reset(new DbwNode(getNodeHandle(), getPrivateNodeHandle()));
  }

private:
  boost::shared_ptr<DbwNode> node_;
};

} // namespace dbw_mkz_can

// Register this plugin with pluginlib.  Names must match nodelets.xml.
//
// parameters: package, class name, class type, base class type
PLUGINLIB_DECLARE_CLASS(dbw_mkz_can, DbwNodelet, dbw_mkz_can::DbwNodelet, nodelet::Nodelet);
