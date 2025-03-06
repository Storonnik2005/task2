#ifndef BACKUP_HPP
#define BACKUP_HPP

#include <string>
#include <vector>
#include <ctime>
#include <stdexcept>

// Класс для объекта резервного копирования (файл или данные)
class BackupObject {
public:
    BackupObject(const std::string& path) : path_(path) {}
    std::string getPath() const { return path_; }
private:
    std::string path_;
};

// Интерфейс для хранилища
class IStorage {
public:
    virtual void save(const std::string& data, const std::string& destination) = 0;
    virtual ~IStorage() = default;
};

// Интерфейс для алгоритма резервного копирования
class IBackupAlgorithm {
public:
    virtual void execute(const std::vector<BackupObject>& objects, IStorage* storage) = 0;
    virtual ~IBackupAlgorithm() = default;
};

// Класс для точки восстановления
class RestorePoint {
public:
    RestorePoint(const std::vector<BackupObject>& objects)
        : objects_(objects), timestamp_(std::time(nullptr)) {}
    std::time_t getTimestamp() const { return timestamp_; }
    std::vector<BackupObject> getObjects() const { return objects_; }
private:
    std::vector<BackupObject> objects_;
    std::time_t timestamp_;
};

// Класс для джобы резервного копирования
class BackupJob {
public:
    BackupJob(const std::string& name, IStorage* storage, IBackupAlgorithm* algorithm);
    void addObject(const BackupObject& obj);
    void removeObject(const std::string& path);
    void createRestorePoint();
    std::vector<RestorePoint> getRestorePoints() const;
private:
    std::string name_;
    std::vector<BackupObject> objects_;
    std::vector<RestorePoint> restorePoints_;
    IStorage* storage_;
    IBackupAlgorithm* algorithm_;
};

#endif