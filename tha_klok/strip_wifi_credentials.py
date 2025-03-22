def commit_callback(commit):
    if commit.original_id.startswith(b'9a79d76'):
        commit.file_changes = [
            fc for fc in commit.file_changes
            if fc.path != b'wifi_credentials.h'
        ]
