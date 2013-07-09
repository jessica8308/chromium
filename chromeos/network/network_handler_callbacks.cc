// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chromeos/network/network_handler_callbacks.h"

#include "base/logging.h"
#include "base/values.h"
#include "chromeos/network/network_event_log.h"

namespace chromeos {
namespace network_handler {

// These are names of fields in the error data dictionary for ErrorCallback.
const char kErrorName[] = "errorName";
const char kErrorDetail[] = "errorDetail";
const char kDbusErrorName[] = "dbusErrorName";
const char kDbusErrorMessage[] = "dbusErrorMessage";
const char kPath[] = "path";

base::DictionaryValue* CreateErrorData(const std::string& service_path,
                                       const std::string& error_name,
                                       const std::string& error_detail) {
  return CreateDBusErrorData(service_path, error_name, error_detail, "", "");
}

base::DictionaryValue* CreateDBusErrorData(
    const std::string& path,
    const std::string& error_name,
    const std::string& error_detail,
    const std::string& dbus_error_name,
    const std::string& dbus_error_message) {
  base::DictionaryValue* error_data(new base::DictionaryValue);
  error_data->SetString(kErrorName, error_name);
  error_data->SetString(kErrorDetail, error_detail);
  error_data->SetString(kDbusErrorName, dbus_error_name);
  error_data->SetString(kDbusErrorMessage, dbus_error_message);
  if (!path.empty())
    error_data->SetString(kPath, path);
  return error_data;
}

void ShillErrorCallbackFunction(const std::string& error_name,
                                const std::string& path,
                                const ErrorCallback& error_callback,
                                const std::string& dbus_error_name,
                                const std::string& dbus_error_message) {
  std::string detail;
  if (!path.empty())
    detail += path + ": ";
  detail += dbus_error_name;
  if (!dbus_error_message.empty())
    detail += ": " + dbus_error_message;
  NET_LOG_ERROR(error_name, detail);

  if (error_callback.is_null())
    return;
  scoped_ptr<base::DictionaryValue> error_data(
      CreateDBusErrorData(path, error_name, detail,
                          dbus_error_name, dbus_error_message));
  error_callback.Run(error_name, error_data.Pass());
}

}  // namespace network_handler
}  // namespace chromeos
