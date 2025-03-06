#include "Backup.hpp"
#include <iostream>

BackupJob::BackupJob(const std::string& name, IStorage* storage, IBackupAlgorithm* algorithm)
    : name_(name), storage_(storage), algorithm_(algorithm) {}

void BackupJob::addObject(const BackupObject& obj) {
    objects_.push_back(obj);
}

void BackupJob::removeObject(const std::string& path) {
    objects_.erase(
        std::remove_if(objects_.begin(), objects_.end(),
            [&path](const BackupObject& obj) { return obj.getPath() == path; }),
        objects_.end());
}

void BackupJob::createRestorePoint() {
    if (objects_.empty()) {
        throw std::runtime_error("No objects to backup");
    }
    algorithm_->execute(objects_, storage_);
    restorePoints_.emplace_back(objects_);
}

std::vector<RestorePoint> BackupJob::getRestorePoints() const {
    return restorePoints_;
}
class FileStorage : public IStorage {
public:
    void save(const std::string& data, const std::string& destination) override {
        std::cout << "Saving " << data << " to " << destination << std::endl;
    }
};

class SplitStorageAlgorithm : public IBackupAlgorithm {
public:
    void execute(const std::vector<BackupObject>& objects, IStorage* storage) override {
        for (const auto& obj : objects) {
            storage->save("Backup of " + obj.getPath(), obj.getPath() + ".backup");
        }
    }
};

class SingleStorageAlgorithm : public IBackupAlgorithm {
public:
    void execute(const std::vector<BackupObject>& objects, IStorage* storage) override {
        std::string combinedData = "Combined backup: ";
        for (const auto& obj : objects) {
            combinedData += obj.getPath() + " ";
        }
        storage->save(combinedData, "single_backup.archive");
    }
};