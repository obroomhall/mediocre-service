# Pulls an image from docker hub and re-tags it for local development

$repo = "obroomhall/mediocre"
$tagToPull = Read-Host -Prompt "Which tag would you like to pull? [base-dev-master]"

if (!$tagToPull) {
    $tagToPull = "base-dev-master"
}

docker pull ${repo}:${tagToPull}

if ($?) {
    Write-Host "Retagging ${repo}:${tagToPull} as mediocre:base-dev-local"
    docker tag ${repo}:${tagToPull} mediocre:base-dev-local
}
